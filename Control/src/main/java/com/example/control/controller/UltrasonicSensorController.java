package com.example.control.controller;

import com.example.control.model.Car;
import com.example.control.model.CarDAO;
import javafx.animation.AnimationTimer;
import javafx.fxml.FXML;
import javafx.scene.chart.ScatterChart;
import javafx.scene.chart.XYChart;

public class UltrasonicSensorController {
    @FXML
    private ScatterChart<String, Double> scatterChart;
    @FXML
    private XYChart.Series<String, Double> series;
    @FXML
    private CarDAO carDAO = new CarDAO();
    @FXML
    private int count = 1;
    @FXML
    private AnimationTimer animationTimer;

    @FXML
    private Car car = new Car("192.168.1.1", 81);

    @FXML
    public void initialize(){
        carDAO.connect(car);
        while (!carDAO.isConnected()){
            System.out.println("Not connected");
        }
        series = new XYChart.Series<>();
        scatterChart.getData().add(series);

        animationTimer = new AnimationTimer() {
            @Override
            public void handle(long now) {
                updateChartData();
            }
        };
        animationTimer.start();
    }
    private void updateChartData() {
        // Check if new data is available
        if (car.isNewData()) {
            series.getData().add(new XYChart.Data<>(String.valueOf(++count), car.getUSSensor()));
            car.setNewData(false); // Resets new data to false
        }

        // Limit the number of data points to display
        if (series.getData().size() > 200) {
            series.getData().remove(0);
        }
    }


}