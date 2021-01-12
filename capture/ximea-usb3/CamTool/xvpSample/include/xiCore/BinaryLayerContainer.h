#ifndef BINARYLAYERCONTAINER_H
#define BINARYLAYERCONTAINER_H

#include <QMap>
#include <xiCoreGlobal.h>
#include <BinaryLayer.h>

typedef QMap<TxBinaryLayerID, CxBinaryLayer*> TxBinaryLayerMap;

class XICORE_API CxBinaryLayerContainer
{
public:
    CxBinaryLayerContainer();
    ~CxBinaryLayerContainer();

    //Working with binary layers
public:
    TxBinaryLayerID addLayer(CxBinaryLayer *pLayer, bool bSetAutoColor = true); //!< Add new layer and assign ID.
    void deleteLayer(TxBinaryLayerID idLayer);              //!< Deletes (and dallocates) the layer with given ID.
    void deleteAllLayers();                                 //!< Deletes all layers from container. \sa deleteLayer()
    CxBinaryLayer* removeLayer(TxBinaryLayerID idLayer);    //!< Remove the layer from container and returns its pointer.
    QList<TxBinaryLayerID> listLayers() const;              //!< List of all layers.
    CxBinaryLayer* layerWithId(TxBinaryLayerID idLayer) const;      //!< Returns a pointer to the layer with given id. If it not exists then it returns NULL.
    bool hasVisibleLayer() const;                           //!< Returns true if at least one layer is visible
    QList<TxBinaryLayerID> listVisibleLayers() const;       //!< List of all visible layers

    void saveLayersVisibilityState();       //!< Saves the per-layer visibility state for later.
    void restoreLayersVisibilityState();    //!< Restores the layers visibility state as saved in saveLayersVisibilityState();
    void hideAllLayers();                   //!< Sets all layers as invisible.

private:
    QColor autoColor();
private:
    TxBinaryLayerMap m_mapLayers;
    QColor m_colCurrent;        //!< For auto color.
    int m_idMax;                //!< ID generator
    QMap<TxBinaryLayerID, bool> m_mapVisibilityStates;
};

#endif // BINARYLAYERCONTAINER_H

