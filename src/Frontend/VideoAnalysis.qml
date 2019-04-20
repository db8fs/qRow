import QtQuick 2.0
import QtMultimedia 5.8


Item {
    width: 640
    height: 480
    y: parent.height - height
    x: parent.width - width

    Camera {
        id: camera

        imageProcessing.whiteBalanceMode: CameraImageProcessing.WhiteBalanceFlash

        exposure {
            exposureCompensation: -1.0
            exposureMode: Camera.ExposurePortrait
        }

        flash.mode: Camera.FlashRedEyeReduction

        imageCapture {
            onImageCaptured: {
                photoPreview.source = preview  // Show the preview in an Image
            }
        }
    }

    VideoOutput {
        source: camera
        filters: [ openCVFilter ]
        anchors.fill: parent
        focus : visible // to receive focus and capture key events when visible
    }

    Image {
        id: photoPreview
    }

    Canvas {
        id: faces
        anchors.fill: parent

        property rect faceRegion

        onPaint: {
            console.log( "Faces: " + faceRegion );

            var ctx = faces.getContext('2d');

            ctx.reset()
            ctx.strokeStyle = Qt.rgba( 255, 0, 0, 1);
            ctx.strokeRect( faceRegion.left, faceRegion.top, faceRegion.width, faceRegion.height );

            ctx.stroke()
        }

        Connections {
            target: faceDetector
            onFaceDetected: {
                faces.faceRegion = faceRegion
                faces.requestPaint()
            }
        }

    }

}


