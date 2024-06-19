#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include "ftxui/screen/screen.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/screen_interactive.hpp" 
#include "ftxui/component/captured_mouse.hpp"
#include "weatherOutput.cpp"
#include <string>
#include <vector>

struct weather_parameters{
    std::vector<double> temperature;
    std::vector<std::string> day;
    std::vector<uint8_t> weathercode;
    std::string design_temperature;
    std::vector<int> wind_speed;
    std::string wind_speed_units;
};

struct weatherOptions {
  std::vector<std::string> cities;
  int amount_of_day;
  int frequency;
};

std::pair<double, double> getCoordinate(std::string city_name, std::string api_key) {
  using json = nlohmann::json;

  cpr::Response response = cpr::Get(
    cpr::Url{"https://api.api-ninjas.com/v1/city"},
    cpr::Parameters{ {"name", city_name} },
    cpr::Header{ {"X-Api-Key", api_key} }
    );

  if (response.status_code == 200) {
      json data = json::parse(response.text.substr(1, response.text.size() - 2));
      std::pair coordinate{data["latitude"], data["longitude"]};
      return coordinate;
  }
  return std::pair(0, 0);
}

int getWeather(double latitude, double longitude, int amount_day, weather_parameters& current_weather) {
  using json = nlohmann::json;
    cpr::Response response = cpr::Get(
        cpr::Url{"https://api.open-meteo.com/v1/forecast"},
        cpr::Parameters{ 
            {"latitude", std::to_string(latitude)}, 
            {"longitude", std::to_string(longitude)},
            {"forecast_days", std::to_string(amount_day)},
            {"hourly", "weather_code"},
            {"hourly", "wind_speed_10m"},
            {"daily", "weathercode"},
            {"hourly", "temperature_2m"}
        }
    );

  if (response.status_code == 200) {
      json data =  json::parse(response.text);
      
      for (int i = 2; i < amount_day * 24; i += 6) {
          current_weather.temperature.push_back(data["hourly"]["temperature_2m"][i]);
      }

      for (int i = 2; i < amount_day * 24; i += 6) {
          current_weather.weathercode.push_back(data["hourly"]["weather_code"][i]);
      }

      for (std::string day: data["daily"]["time"]) {
          current_weather.day.push_back(day);
      }

      for (int i = 2; i < amount_day * 24; i += 6) {
          current_weather.wind_speed.push_back(data["hourly"]["wind_speed_10m"][i]);
      }
      
      current_weather.design_temperature = data["hourly_units"]["temperature_2m"];
      current_weather.wind_speed_units = data["hourly_units"]["wind_speed_10m"];
      return 0;
  } 
  return 1;
}

int updateConsole(weatherOptions weather_options, std::string key, weather_parameters& current_weather, std::pair<double, double>& coordinate) {
  using namespace ftxui;
  int current_city = 0;
  Element document;
  ScreenInteractive screen = ScreenInteractive::TerminalOutput();
  int current = 0;
  output output_;
  std::string temperature;
  std::string wind_speed;
  weather_parameters new_weather;
  int exit_code = 0;

  Component renderer = Renderer([&] () {
  std::vector<Element> days;

  for (int current_day = 0; current_day < weather_options.amount_of_day; ++current_day) {
    std::vector<Element> day_;

    temperature = std::to_string(current_weather.temperature[4 * current_day]);
    wind_speed = std::to_string(current_weather.wind_speed[4 * current_day]);
    Element night = window(text("Night") | center,
    vbox({
      text(output_.weather_message[current_weather.weathercode[4 * current_day]]),
      text(""),
      dbox(text(output_.weather_image[current_weather.weathercode[4 * current_day]][0]), text(temperature.substr(0, temperature.find('.') + 2) + current_weather.design_temperature) | center),
      dbox(text(output_.weather_image[current_weather.weathercode[4 * current_day]][1]), text(wind_speed + " " + current_weather.wind_speed_units) | center),
      text(output_.weather_image[current_weather.weathercode[4 * current_day]][2]),
      text(output_.weather_image[current_weather.weathercode[4 * current_day]][3]),
      text(output_.weather_image[current_weather.weathercode[4 * current_day]][4])
    }) | size(WIDTH, EQUAL, std::max(screen.dimx() / 4, 150)) | xflex_shrink);
    day_.push_back(night);

    temperature = std::to_string(current_weather.temperature[4 * current_day + 1]);
    wind_speed = std::to_string(current_weather.wind_speed[4 * current_day + 1]);
    Element morning = window(text("Morning") | center,
    vbox({
      text(output_.weather_message[current_weather.weathercode[4 * current_day + 1]]),
      text(""),
      dbox(text(output_.weather_image[current_weather.weathercode[4 * current_day + 1]][0]), text(temperature.substr(0, temperature.find('.') + 2) + current_weather.design_temperature) | center),
      dbox(text(output_.weather_image[current_weather.weathercode[4 * current_day + 1]][1]), text(wind_speed + " " + current_weather.wind_speed_units) | center),
      text(output_.weather_image[current_weather.weathercode[4 * current_day + 1]][2]),
      text(output_.weather_image[current_weather.weathercode[4 * current_day + 1]][3]),
      text(output_.weather_image[current_weather.weathercode[4 * current_day + 1]][4])
    }) | size(WIDTH, EQUAL, std::max(screen.dimx() / 4, 150)) | xflex_shrink);
    day_.push_back(morning);

    temperature = std::to_string(current_weather.temperature[4 * current_day + 2]);
    wind_speed = std::to_string(current_weather.wind_speed[4 * current_day + 2]);
    Element day = window(text("Day") | center,
    vbox({
      text(output_.weather_message[current_weather.weathercode[4 * current_day + 2]]),
      text(""),
      dbox(text(output_.weather_image[current_weather.weathercode[4 * current_day + 2]][0]), text(temperature.substr(0, temperature.find('.') + 2) + current_weather.design_temperature) | center),
      dbox(text(output_.weather_image[current_weather.weathercode[4 * current_day + 2]][1]), text(wind_speed + " " + current_weather.wind_speed_units) | center),
      text(output_.weather_image[current_weather.weathercode[4 * current_day + 2]][2]),
      text(output_.weather_image[current_weather.weathercode[4 * current_day + 2]][3]),
      text(output_.weather_image[current_weather.weathercode[4 * current_day + 2]][4]),
    }) | size(WIDTH, EQUAL, std::max(screen.dimx() / 4, 150)) | xflex_shrink);
    day_.push_back(day);

    temperature = std::to_string(current_weather.temperature[4 * current_day + 3]);
    wind_speed = std::to_string(current_weather.wind_speed[4 * current_day + 3]);
    Element evening = window(text("Evening") | center,
    vbox({
      text(output_.weather_message[current_weather.weathercode[4 * current_day + 3]]),
      text(""),
      dbox(text(output_.weather_image[current_weather.weathercode[4 * current_day + 3]][0]), text(temperature.substr(0, temperature.find('.') + 2) + current_weather.design_temperature) | center),
      dbox(text(output_.weather_image[current_weather.weathercode[4 * current_day + 3]][1]), text(wind_speed + " " + current_weather.wind_speed_units) | center),
      text(output_.weather_image[current_weather.weathercode[4 * current_day + 3]][2]),
      text(output_.weather_image[current_weather.weathercode[4 * current_day + 3]][3]),
      text(output_.weather_image[current_weather.weathercode[4 * current_day + 3]][4])
    }) | size(WIDTH, EQUAL, std::max(screen.dimx() / 4, 150)) | xflex_shrink);
    day_.push_back(evening);

    days.push_back(window(text(current_weather.day[current_day]) | center | bold, hbox(day_)));
  }

  document = vbox(text(weather_options.cities[current_city]) | bold, vbox(days));
  return document;
});



Component component = CatchEvent(renderer, [&](Event event) {
  if (event == Event::Escape) {
    screen.ExitLoopClosure()();
    return true;
  } else if (event == Event::Character('n')) {
    current_city = (current_city + 1) % weather_options.cities.size();

    coordinate = getCoordinate(weather_options.cities[current_city], key);

    if (coordinate.first == 0 && coordinate.second == 0) {
      screen.ExitLoopClosure()();
      exit_code = 1;
      return true;
    }

    new_weather = {};
    if (getWeather(coordinate.first, coordinate.second, weather_options.amount_of_day, new_weather)) {
      screen.ExitLoopClosure()();
      exit_code = 1;
      return true;
    }
    current_weather = new_weather;

    screen.PostEvent(Event::Custom);

    return true;
  } else if (event == Event::Character('p')) {
    current_city = (current_city - 1) % weather_options.cities.size();

    coordinate = getCoordinate(weather_options.cities[current_city], key);

    if (coordinate.first == 0 && coordinate.second == 0) {
      screen.ExitLoopClosure()();
      exit_code = 1;
      return true;
    }

    new_weather = {};
    if (getWeather(coordinate.first, coordinate.second, weather_options.amount_of_day, new_weather)) {
      screen.ExitLoopClosure()();
      exit_code = 1;
      return true;
    }
    current_weather = new_weather;

    screen.PostEvent(Event::Custom);
    return true;
  } else if (event == Event::Character('+') || event == Event::Character('=')) {
    
    new_weather = {};
    
    if ((current_weather.day.size() < weather_options.amount_of_day + 1) && (getWeather(coordinate.first, coordinate.second, std::min(weather_options.amount_of_day + 1, 16), new_weather) == 0)) {
      weather_options.amount_of_day = std::min(weather_options.amount_of_day + 1, 16);
      current_weather = new_weather;
    } else {
      weather_options.amount_of_day = std::min(weather_options.amount_of_day + 1, static_cast<int>(current_weather.day.size()));
    }

    screen.PostEvent(Event::Custom);
    return true;
  } else if (event == Event::Character('-')) {
    weather_options.amount_of_day = std::max(weather_options.amount_of_day - 1, 1);
    screen.PostEvent(Event::Custom);
    return true;
  }

  return false;
  });

  screen.Loop(component);

  return exit_code;
}
