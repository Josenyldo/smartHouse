import 'dart:async';
import 'package:flutter/material.dart';
import 'package:firebase_database/firebase_database.dart';
import 'package:flutter/services.dart';

class IotScreen extends StatefulWidget {
  @override
  _IotScreenState createState() => _IotScreenState();
}

class _IotScreenState extends State<IotScreen>
    with SingleTickerProviderStateMixin {
  @override
  final dbRef = FirebaseDatabase()
      .reference()
      .child('Vazao'); //FirebaseDatabase.instance.reference();
  bool value = false;
  Color color = Colors.grey;

  onUpdate() {
    setState(() {
      readData();
      value = !value;
    });
  }

  @override
  Widget build(BuildContext context) {
    SystemChrome.setPreferredOrientations([
      DeviceOrientation.portraitUp,
      DeviceOrientation.portraitDown,
    ]);
    return MaterialApp(
        theme: ThemeData(
          brightness: Brightness.dark,
        ),
        debugShowCheckedModeBanner: false,
        home: Scaffold(
          body: SafeArea(
            child: StreamBuilder(
              stream: dbRef.onValue,
              builder: (BuildContext context, AsyncSnapshot<Event> snapshot) {
                if (snapshot.hasData &&
                    !snapshot.hasError &&
                    snapshot.data != null) {
                  return Column(
                    children: [
                      Padding(
                        padding: const EdgeInsets.all(18.0),
                        child: Row(
                          mainAxisAlignment: MainAxisAlignment.center,
                          children: <Widget>[
                            Text("Smart House",
                                style: TextStyle(
                                    color: Colors.white,
                                    fontSize: 20,
                                    fontWeight: FontWeight.bold)),
                          ],
                        ),
                      ),
                      SizedBox(height: 20),
                      Row(
                        mainAxisAlignment: MainAxisAlignment.center,
                        children: [
                          Column(
                            children: [
                              Padding(
                                padding: const EdgeInsets.all(8.0),
                                child: Text("Vazão de água",
                                    style: TextStyle(
                                        color: !value
                                            ? Colors.white
                                            : Colors.yellow,
                                        fontSize: 20,
                                        fontWeight: FontWeight.bold)),
                              ),
                              Padding(
                                padding: const EdgeInsets.all(8.0),
                                child: Text(
                                    snapshot.data!.snapshot.value["Data"]
                                            .toString() +
                                        " L/s",
                                    style: TextStyle(
                                        color: Colors.white, fontSize: 20)),
                              ),
                            ],
                          ),
                        ],
                      ),
                      SizedBox(height: 80),
                      Padding(
                        padding: const EdgeInsets.all(18.0),
                        child: FloatingActionButton.extended(
                          icon: value
                              ? Icon(Icons.visibility)
                              : Icon(Icons.visibility_off),
                          backgroundColor: value ? Colors.yellow : Colors.white,
                          label: value ? Text("ON") : Text("OFF"),
                          elevation: 20.00,
                          onPressed: () {
                            onUpdate();
                            writeData();
                          },
                        ),
                      ),
                    ],
                  );
                }
                return Container();
              },
            ),
          ),
        ));
  }

  Future<void> writeData() async {
    dbRef.child("Porta").set({"switch": !value});
  }

  Future<void> readData() async {
    dbRef.child("Vazao").once().then((DataSnapshot snapshot) {});
  }
}
