#ifndef VECTORCONTAINER_H
#define VECTORCONTAINER_H

#include <QMap>
#include <QRectF>
#include <xiCoreGlobal.h>

class QGraphicsScene;
class CxVectorObject;

typedef QMap<TxVecObjID, CxVectorObject*> TxVectorObjectMap;

//---------------------------------------------------------------------------
//! \brief Our special class above QGraphicsScene maintains list of our vector objects and assigns IDs to them.
class XICORE_API CxVecObjContainer
{
public:
    explicit CxVecObjContainer(QGraphicsScene *pScene);

    QRectF sceneRect() const;

    // working with objects
public:
    TxVecObjID addObject(CxVectorObject *pObj);     //!< Add object to scene, and assign it a new ID
    void deleteObject(TxVecObjID idObj);            //!< Removes the object from scene and deletes the object
    void deleteAllObjects();                        //!< Removes all our vector object from scene
    QVector<TxVecObjID> listAllObjects();           //!< Returns the list of all objects in the scene

    CxVectorObject* objectWithId(TxVecObjID idObj); //!< Returns pointer from ID

    void scaleObjects(double sx, double sy, bool bReflectConstraints = true);
    void moveObjects(double dx, double dy, bool bReflectConstraints = true);

    // object visibility helpers
public:
    void saveObjVisibilityState();      //!< Save the per-object visility for later
    void restoreObjVisibilityState();   //!< Restores the state as saved in saveObjVisibilityState
    void hideAllObjects();              //!< Sets all vector objects as invisible

protected:
    QGraphicsScene *m_pScene;       //!< Each container has to be connected to a scene
    TxVecObjID m_idMax;             //!< Object ID generator
    TxVectorObjectMap m_mapObjs;    //!< Mapping from object id to the pointer
    QMap<TxVecObjID, bool> m_mapObjVisibilityState; //!< Saved visibility state using saveObjVisibilityState

};

//---------------------------------------------------------------------------
#endif // VECTORCONTAINER_H
