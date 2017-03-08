import QtQuick 2.0
import Qt3D 2.0
import Qt3D.Shapes 2.0
import CarModel 1.0


Viewport  {
    width: parent.width
    height: parent.height
    fillColor: "transparent"

    //camera: Camera { eye: Qt.vector3d(21,7,19)}
//! [1]

//! [2]
    Item3D {
        //transform: LookAt { subject: focalPenguin}
        position: Qt.vector3d(0, 1.7, -6)

        CarItem3D {
            id: monkey
            scale: 1
            mesh: Mesh { source: "res/meshes/car4.3ds"; options: "ForceSmooth"}
            //effect: Effect { material: gold}
            //transform: Rotation3D {angle: 90; axis: Qt.vector3d(1,0,0);}
        }
    }
}
