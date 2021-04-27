#!/usr/bin/env python3

import numpy as np
from pathlib import Path
import pickle
from PIL import Image
from scipy import optimize
from scipy.linalg import svd
import time
import glob   # for latest file
import os    

# get the latest *.tif file in the queue, and use that for recon
list_of_files = glob.glob('/media/msata/queue/*.tif') # * means all if need specific format then *.csv
latest_file = max(list_of_files, key=os.path.getctime)
print(latest_file)


# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ #

# NEED TO UPDATE FILEPATH TO CORRECT LOCATION ON THE CHIMP; GRABS CWD AND ASSUMES CALIBRATION FILES ARE THERE
calibration_folder = "/home/labuser/calibration_files/" #Path.cwd()  # This is the folder where the calibration data is saved

# NEED TO UPDATE TEMPORARY FILEPATH TO THE CORRECT THING!!!!  - This is the file for spectral reconstruction
#temporary_folder_for_recon = Path("C:\\Users\\cmann\\Documents\\Nanohmics\\ISSNL Hyperspectral\\Calibration\\flight_articles\\SN5\\val\\200 col notch 50 step 100ms int sn5 post tcycle 2\\0600")
temporary_folder_for_recon = "/media/msata/queue"
temporary_filename = latest_file #hs_550nm_2021.04.22-14.21.51.359.tif"

# NEED TO SET THE CORRECT EXPORT LOCATION:
export_filepath = "/media/msata/queue" #calibration_folder  # This is where we want the reconstructed data exported to
export_filename = str( os.path.splitext(latest_file)[0] + ".npz")  # This is what we want to name the reconstructed data (npz filename needed)

# Region of frame to use for background subtraction fitting:
darkframe_region_slice = slice(5, 35), slice(5, 35)

# Number of useful scene elements:
num_rows = 59
num_cols = 118
scene_element_dimension = 17

# Default filenames:
cal_filename = "SN5_complete_calibration.npz"
se_filename = "s_e_dict.pkl"

# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ #
# Support functions


# Need to load pickled scene element location dictionary:
def load_dict(filename):
    with open(filename, 'rb') as f:
        ret_di = pickle.load(f)
    return ret_di


def import_image(filename):
    with Image.open(filename) as img:
        img_arr = np.array(img)
    return img_arr


def scene_element_slicer(r_i, c_i, scene_element_position_dict, scene_element_dimension, as_slice=False):
    """
    Returns the starting and ending row and column indices needed to slice pixels from a scene element.

    Parameters
    ----------
    r_i
        Scene element row
    c_i
        Scene element column
    scene_element_position_dict
        Lookup table
    scene_element_dimension
        Size of scene element
    as_slice
        Whether to return as an actual slice, or just the start/end indices

    Returns
    -------
    row, col slices
    """
    dx = scene_element_dimension // 2
    row, col = scene_element_position_dict[r_i, c_i]
    r1, r2 = int(row) - dx, int(row) + dx + 1
    c1, c2 = int(col) - dx, int(col) + dx + 1
    if as_slice:
        return slice(r1, r2), slice(c1, c2)
    else:
        return r1, r2, c1, c2


def nnls_reconstruction(cal_speckle: np.ndarray,
                        speckle_pattern: np.ndarray):
    """

    Parameters
    ----------
    cal_speckle
        Stack of speckle from calibration
    speckle_pattern
        Speckle for reconstruction

    Returns
    -------
    coeffs, res
    """

    t_mat = np.reshape(cal_speckle, (len(cal_speckle), -1))
    speckle_pattern = speckle_pattern.flatten()
    # Bad pixel mask:
    bpm = ~np.isfinite(speckle_pattern) | np.any(~np.isfinite(t_mat), axis=0)

    t_mat = t_mat[:, ~bpm]
    speckle_pattern = speckle_pattern[~bpm]

    try:
        new_nnls_soln, residues = optimize.nnls(t_mat.T,
                                                speckle_pattern,
                                                maxiter=10 * t_mat.shape[1])
    except RuntimeError or ValueError:
        new_nnls_soln, residues = None, None

    return new_nnls_soln, residues


def pseudo_inverse_soln(cal_speckle: np.ndarray,
                        speckle_pattern: np.ndarray):
    t_mat = np.reshape(cal_speckle, (len(cal_speckle), -1))
    speckle_pattern = speckle_pattern.flatten()
    u, s, vh = svd(t_mat, full_matrices=False)
    pseudo_inverse = np.matmul(np.matmul(np.transpose(vh), np.diag(1 / s)), np.transpose(u))
    return np.matmul(pseudo_inverse.T, speckle_pattern)


def linfit(x_val_arr, y_val_arr):
    x_vect = y_val_arr
    y_vect = x_val_arr
    A_mat = np.vstack([x_vect, np.ones(len(x_vect))]).T
    slope, offset = np.linalg.lstsq(A_mat, y_vect, rcond=None)[0]
    return slope, offset


# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ #
# Import calibration files from hard drive:

# Baseline the runtime:
t_0 = time.time()

#imported_files = np.load(calibration_folder / cal_filename)
imported_files = np.load(calibration_folder + cal_filename)

image_stack = imported_files['image_stack']
spectra_stack = imported_files['spectra_stack']
spectral_domain = imported_files['spectral_domain']
dark_spectrum = imported_files['dark_spectrum']
dark_frame = imported_files['dark_frame']

#all_scene_elements = load_dict(calibration_folder / se_filename)
all_scene_elements = load_dict(calibration_folder + se_filename)


# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ #
# Need to have an image to perform calibration on...  for now:

#image_for_reconstruction = import_image(temporary_folder_for_recon / temporary_filename)
image_for_reconstruction = import_image( temporary_filename)

slope, offset = linfit(dark_frame[darkframe_region_slice].flatten(), image_for_reconstruction[darkframe_region_slice].flatten())

test_image_bg_sub = image_for_reconstruction - (dark_frame - offset) / slope

intensity_map = np.zeros((num_rows, num_cols))
nnls_map = np.zeros((num_rows, num_cols, len(spectra_stack)))
pinv_map = np.zeros_like(nnls_map)
for r in range(num_rows):
    for c in range(num_cols):
        se_slice = scene_element_slicer(r, c, all_scene_elements, scene_element_dimension, as_slice=True)

        local_tmatrix = image_stack[:, se_slice[0], se_slice[1]]
        local_image_data = test_image_bg_sub[se_slice]

        intensity_map[r, c] = np.nansum(local_image_data)

        coeffs, res = nnls_reconstruction(local_tmatrix, local_image_data)
        if coeffs is None:
            coeffs = np.zeros(len(spectra_stack))

        nnls_map[r, c] = coeffs

        pinv_coeffs = pseudo_inverse_soln(local_tmatrix, local_image_data)

        pinv_map[r, c] = pinv_coeffs

        nnls_spec = np.dot(spectra_stack.T, coeffs)
        pinv_spec = np.dot(spectra_stack.T, pinv_coeffs)


np.savez_compressed(export_filepath/export_filename, intensity_map=intensity_map, nnls_map=nnls_map, pinv_map=pinv_map)

t_1 = time.time()

print("Total run time: ", t_1 - t_0)

