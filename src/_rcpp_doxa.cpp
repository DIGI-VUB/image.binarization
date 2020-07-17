#include <Rcpp.h>
// [[Rcpp::plugins(cpp17)]]

#include "Doxa/Algorithm.hpp"
#include "Doxa/Bataineh.hpp"
#include "Doxa/Bernsen.hpp"
//#include "Doxa/ClassifiedPerformance.hpp"
#include "Doxa/ContrastImage.hpp"
#include "Doxa/Convolution.hpp"
//#include "Doxa/DRDM.hpp"
#include "Doxa/EdgeDetector.hpp"
#include "Doxa/Gatos.hpp"
#include "Doxa/Grayscale.hpp"
#include "Doxa/Image.hpp"
#include "Doxa/ISauvola.hpp"
#include "Doxa/LocalWindow.hpp"
#include "Doxa/MeanCalculator.hpp"
#include "Doxa/MeanVarianceCalculator.hpp"
#include "Doxa/Morphology.hpp"
#include "Doxa/Niblack.hpp"
#include "Doxa/Nick.hpp"
#include "Doxa/Otsu.hpp"
#include "Doxa/Palette.hpp"
#include "Doxa/Parameters.hpp"
//#include "Doxa/PNM.hpp"
#include "Doxa/Region.hpp"
#include "Doxa/Sauvola.hpp"
#include "Doxa/Su.hpp"
#include "Doxa/TRSingh.hpp"
#include "Doxa/Types.hpp"
#include "Doxa/Wan.hpp"
#include "Doxa/WeinerFilter.hpp"
#include "Doxa/Wolf.hpp"

using namespace Doxa;

inline static int as_int(uint8_t x){ 
  return ((int)x); 
}


Image doxa_read(Rcpp::IntegerVector &x, int height, int width){
  Image image(width, height);
  for(int i=0; i < x.size(); i++){
    image.data[i] = (uint8_t)x[i];
  }
  return image;
}

Parameters doxa_parameters(Rcpp::List opts) {
  Rcpp::Function cl("class");
  
  Parameters parameters;
  int n = opts.size();
  if (n > 0) {
    Rcpp::CharacterVector keys = opts.names();
    for(int i=0; i < keys.size(); i++){
      std::string key = Rcpp::as<std::string>(keys[i]);
      std::string type = Rcpp::as<std::string>(cl(opts[key]));
      if(type == "integer"){
        int value = Rcpp::as<int>(opts[key]);
        parameters.Set(key, value);
      }else{
        double value = Rcpp::as<double>(opts[key]);
        parameters.Set(key, value);
      }
    }
  }else{
    //int w = 0;
    //Parameters parameters(ParameterMap({ { "window", w == 0 ? 75 : w } }));
    parameters.Set("window", 75);
  }
  return parameters;
}


// [[Rcpp::export]]
Rcpp::List doxa_binary(Rcpp::IntegerVector x, int height, int width, std::string type, Rcpp::List opts) {
  // Copy image in Doxa format
  Image image = doxa_read(x, height, width);
  // Setup params
  Parameters parameters = doxa_parameters(opts);
  // Apply the logic
  Image binaryImage;
  if(type == "otsu"){
    binaryImage = Otsu::ToBinaryImage(image);
  }else if(type == "bernsen"){
    binaryImage = Bernsen::ToBinaryImage(image, parameters);
  }else if(type == "niblack"){
    binaryImage = Niblack::ToBinaryImage(image, parameters);
  }else if(type == "sauvola"){
    binaryImage = Sauvola::ToBinaryImage(image, parameters);
  }else if(type == "wolf"){
    binaryImage = Wolf::ToBinaryImage(image, parameters);
  }else if(type == "nick"){
    binaryImage = Nick::ToBinaryImage(image, parameters);
  }else if(type == "gatos"){
    binaryImage = Gatos::ToBinaryImage(image, parameters);
  }else if(type == "su"){
    binaryImage = Su::ToBinaryImage(image, parameters);
  }else if(type == "trsingh"){
    binaryImage = TRSingh::ToBinaryImage(image, parameters);
  }else if(type == "bataineh"){
    binaryImage = Bataineh::ToBinaryImage(image);
  }else if(type == "wan"){
    binaryImage = Wan::ToBinaryImage(image, parameters);
  }else if(type == "isauvola"){
    binaryImage = ISauvola::ToBinaryImage(image, parameters);
  }
  // Overwrite existing set of 0-255 values as output
  for(int i=0; i < binaryImage.size; i++){
    x[i] = as_int(binaryImage.data[i]);
  }
  // Get list of results
  Rcpp::List out = Rcpp::List::create(
    Rcpp::Named("x") = x, 
    Rcpp::Named("height") = height,
    Rcpp::Named("width") = width);
  return out;
}




/*
#include "Doxa/PNM.hpp"
 
// [[Rcpp::export]]
Rcpp::List doxa_example_read(std::string source) {
  const Image image = PNM::Read(source);
  
  Rcpp::IntegerVector out_original(image.size);
  for(int i=0; i < image.size; i++){
    out_original[i] = as_int(image.data[i]);
  }
  Rcpp::List out = Rcpp::List::create(Rcpp::Named("original") = out_original, 
                                      Rcpp::Named("width") = image.width,
                                      Rcpp::Named("height") = image.height,
                                      Rcpp::Named("size") = image.size,
                                      Rcpp::Named("depth") = image.depth,
                                      Rcpp::Named("maxVal") = image.maxVal,
                                      Rcpp::Named("tupleType") = image.tupleType);
  return out;
}

// [[Rcpp::export]]
Rcpp::List doxa_example(std::string source, std::string output) {
  const Image image = PNM::Read(source);
  
  // Setup base params
  int w = 0;
  //Parameters parameters({ {"window", 25}, {"k", 0.10} });
  Parameters parameters(ParameterMap({ { "window", w == 0 ? 75 : w } }));
  
  Image binaryImage = Su::ToBinaryImage(image, parameters);
  PNM::Write(binaryImage, output);
  
  Rcpp::IntegerVector out_original(image.size);
  for(int i=0; i < image.size; i++){
    out_original[i] = as_int(image.data[i]);
  }
  Rcpp::IntegerVector out_binary(binaryImage.size);
  for(int i=0; i < binaryImage.size; i++){
    out_binary[i] = as_int(binaryImage.data[i]);
  }
  
  Rcpp::List out = Rcpp::List::create(Rcpp::Named("original") = out_original, 
                                      Rcpp::Named("binary") = out_binary,
                                      Rcpp::Named("width") = image.width,
                                      Rcpp::Named("height") = image.height);
  return out;
}
 
 */