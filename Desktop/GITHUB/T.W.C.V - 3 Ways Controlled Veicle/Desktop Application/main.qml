import QtQuick 2.5
import QtQuick.Window 2.2
import Client 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("ESRG")


    Client {
        id: client
    }

    MainForm
   {


        Component.onCompleted: { client.connectToHost("192.168.43.2",9000);
                                 client.writeData(5);   //to identify that this client is the Desktop Application
                               }
        Component.onDestruction:  {/*client.writeData(6);*/  }

        upbutton.onPressed:    { client.writeData(1);}    upbutton.onReleased:     { client.writeData(0);}
        downbutton.onPressed:  { client.writeData(2);}    downbutton.onReleased:   { client.writeData(0);}
        rightbutton.onPressed: { client.writeData(3);}    rightbutton.onReleased:  { client.writeData(0);}
        leftbutton.onPressed:  { client.writeData(4);}    leftbutton.onReleased:   { client.writeData(0);}
//{ Qt.quit();}
        anchors.fill: parent

        Item {

            Timer {
                interval: 500; running: true; repeat: true
                onTriggered: {
                    client.readData();
                    time.text  = "X_Angle " + client.gui_Update(1);//Date().toString();
                    time2.text = "Y_Angle " + client.gui_Update(2);
                    time3.text = "Ultrassonic " + client.gui_Update(3);
                    time4.text = "InfraRed " + client.gui_Update(4);
                    }
            }

            Text { x: 250;  y: 60;   id: time;   font.family: "Helvetica"; font.pointSize: 24; color: "red"}
            Text { x: 250;  y: 110;  id: time2;  font.family: "Helvetica"; font.pointSize: 24; color: "red"}
            Text { x: 250;  y: 160;  id: time3;  font.family: "Helvetica"; font.pointSize: 24; color: "red"}
            Text { x: 250;  y: 210;  id: time4;  font.family: "Helvetica"; font.pointSize: 24; color: "red"}

        }


        function myFunction(){
          //text1.text = "Y U NO change!";  //Does nothing
         }


    }


}
