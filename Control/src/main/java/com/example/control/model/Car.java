package com.example.control.model;

public class Car {
    private String ip;
    private int port;
    private double USSensor;
    private boolean newData;

    public Car(){
    }
    public Car(String ip, int port){
        this.ip = ip;
        this.port = port;
    }

    public String getIp() {
        return ip;
    }

    public int getPort() {
        return port;
    }

    public void setIp(String ip) {
        this.ip = ip;
    }

    public void setPort(int port) {
        this.port = port;
    }

    public double getUSSensor() {
        return USSensor;
    }

    public void setUSSensor(double USSensor) {
        this.USSensor = USSensor;
    }

    public boolean isNewData() {
        return newData;
    }

    public void setNewData(boolean newData) {
        this.newData = newData;
    }

    @Override
    public String toString() {
        return "Car{" +
                "ip='" + ip + '\'' +
                ", port=" + port +
                '}';
    }
}
