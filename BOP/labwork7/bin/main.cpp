#include <iostream>
#include <string>
#include <fstream>
#include "lib/weather.cpp"
#include <nlohmann/json.hpp>

int main(int argc, char** argv) {
  using json = nlohmann::json;
  
  std::ifstream input("configure.json");
  json data;
  std::ifstream input_key("key.txt");
  std::string key;
  weatherOptions weather_options;
  std::pair<double, double> coordinate;
  weather_parameters current_weather;
  

  data = json::parse(input);

  std::getline(input_key, key);

  input.close();
  input_key.close();

  weather_options.amount_of_day = data["amount of days"];
  weather_options.frequency = data["frequency"];
  for (auto city: data["cities"]) {
    weather_options.cities.push_back(city);
  }

  coordinate = getCoordinate(weather_options.cities[0], key);

  if (coordinate.first == 0 && coordinate.second == 0) {
    std::cout << "truoble with connect!\n";
    return 1;
  }

  if (getWeather(coordinate.first, coordinate.second, weather_options.amount_of_day, current_weather)) {
    std::cout << "trouble with connect!'\n";
    return 1;
  }

  if (updateConsole(weather_options, key, current_weather, coordinate)) {
    std::cout << "triuble with connect!\n";
    return 1;
  }
  return 0; 
}
