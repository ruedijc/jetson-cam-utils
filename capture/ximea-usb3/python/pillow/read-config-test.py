#!/usr/bin/env python3
import configparser

config = configparser.ConfigParser(allow_no_value=True)
config.sections()
config.read('/etc/hsi-config/hsi_task_a.ini')


hsi_exposure_min = config['hsi']['HSI_EXPOSURE_MIN']
hsi_exposure_max = config['hsi']['HSI_EXPOSURE_MAX']
hsi_exposure_auto = config['hsi']['HSI_EXPOSURE_AUTO']
hsi_exposure_steps = config['hsi']['HSI_EXPOSURE_STEPS']
hsi_samples = config['hsi']['HSI_SAMPLES']
ctx_sync_capture = config['ctx']['ctx_sync_capture']
ctx_img_quality = config['ctx']['ctx_img_quality']
hsi_save_path = config['filesys']['hsi_save_path']
hsi_task_max_disk_gb = config['filesys']['hsi_task_max_disk_gb']


print('hsi-config/hsi_task_a.ini  configuration:')

print(f'Xi Min Exposure: {hsi_exposure_min}')
print(f'Xi Max Exposure: {hsi_exposure_max}')
print(f'Xi Exposure Auto: {hsi_exposure_auto}')
print(f'Xi Exposure Steps: {hsi_exposure_steps}')
print(f'Xi HSI Samples: {hsi_samples}')
print(f'Context sync capture: {ctx_sync_capture}')
print(f'Save Path: {hsi_save_path}')
print(f'Max size on disk in GB: {hsi_task_max_disk_gb}')

