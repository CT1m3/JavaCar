package com.example.control.model;

import com.example.control.controller.UltrasonicSensorController;
import org.java_websocket.client.WebSocketClient;
import org.java_websocket.handshake.ServerHandshake;

import java.net.URI;
import java.net.URISyntaxException;

public class CarDAO {
    private WebSocketClient client;
    private boolean connected = false;
    public void connect(Car car){
        String serverUri = "ws://"+car.getIp()+":"+car.getPort();
        try {
            client = new WebSocketClient(new URI(serverUri)) {
                @Override
                public void onOpen(ServerHandshake serverHandshake) {
                    connected = true;
                    System.out.println("Connected Successfully");
                }

                @Override
                public void onMessage(String s) {
                    car.setUSSensor(Double.parseDouble(s));
                    car.setNewData(true);
                }

                @Override
                public void onClose(int i, String s, boolean b) {

                }
                @Override
                public void onError(Exception e) {

                }
            };
            client.connect();
        } catch (URISyntaxException e) {
            System.err.println("Invalid WebSocket server URI: " + e.getMessage());
        }
    }
    public void send(String str) {
        if (client == null || client.isClosed()){
            System.out.println("WebSocket is not connected!");
        }
        client.send(str);
    }

    public boolean isConnected() {
        return connected;
    }
}
