module com.example.control {
    requires javafx.controls;
    requires javafx.fxml;
    requires org.java_websocket;


    opens com.example.control to javafx.fxml;
    exports com.example.control;
    exports com.example.control.controller;
    opens com.example.control.controller to javafx.fxml;
}