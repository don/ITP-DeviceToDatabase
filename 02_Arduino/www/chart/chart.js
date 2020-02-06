var temperatureContext = document.getElementById("temperatureCanvas").getContext("2d");
var humidityContext = document.getElementById("humidityCanvas").getContext("2d");

var temperatureData = {
  labels: [],
  datasets: [{
    label: "Temperature (°F)",
    backgroundColor: "rgba(0, 100, 0, 0.2)",
    borderColor: "rgba(0, 100, 0, 1)",
    borderWidth: 1,
    pointRadius: 0,
    data: []
  }]
};

var humidityData = {
  labels: [],
  datasets: [{
    label: "Humidity (%)",
    backgroundColor: "rgba(0, 0, 100, 0.2)",
    borderColor: "rgba(0, 0, 100, 1)",
    borderWidth: 1,
    pointRadius: 0,
    data: []
  }]
};

var temperatureChart = new Chart.Line(temperatureContext, {data: temperatureData});
var humidityChart = new Chart.Line(humidityContext, {data: humidityData});
