import QtQuick 2.0

Item {
    id:root
    property real zoom: 1
    property point pMapRef:Qt.point(0,0)
    property point pZoneRef:Qt.point(0,0)


    function getXmap(_x){
        return (_x/zoom) - _translate.x
    }

    function getYmap(_y){
        return (_y/zoom) - _translate.y
    }

    function getCoordMap(p){
        return Qt.point(
                     getXmap(p.x)
                    ,getYmap(p.y)
                    );
    }

    onZChanged: setScale()
    onPMapRefChanged: setScale()
    onPZoneRefChanged: setScale()


    function setScale(){

        _translate.x = (pZoneRef.x/zoom)-pMapRef.x
        _translate.y = (pZoneRef.y/zoom)-pMapRef.y

        console.log("set zoom  zoom "+zoom+" to "+_translate.x+";"+_translate.y)

        _scale.xScale = zoom
        _scale.yScale = zoom
    }



    transform: [
        Translate {id:_translate ; x:0; y:0}
        ,Rotation {id:rot}
        ,Scale {id:_scale}
    ]


    width: 0
    height: 0
    x:0
    y:0
}
