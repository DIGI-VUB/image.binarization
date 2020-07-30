#' @title Binarize Images For Enhancing Optical Character Recognition
#' @description Binarize images in order to further process it for Optical Character Recognition (OCR) or Handwritten Text Recognition (HTR) purposes
#' \itemize{
#' \item Otsu - "A threshold selection method from gray-level histograms", 1979.
#' \item Bernsen - "Dynamic thresholding of gray-level images", 1986.
#' \item Niblack - "An Introduction to Digital Image Processing", 1986.
#' \item Sauvola - "Adaptive document image binarization", 1999.
#' \item Wolf - "Extraction and Recognition of Artificial Text in Multimedia Documents", 2003.
#' \item Gatos - "Adaptive degraded document image binarization", 2005. (Partial)
#' \item NICK - "Comparison of Niblack inspired Binarization methods for ancient documents", 2009.
#' \item Su - "Binarization of Historical Document Images Using the Local Maximum and Minimum", 2010.
#' \item T.R. Singh - "A New local Adaptive Thresholding Technique in Binarization", 2011.
#' \item Bataineh - "An adaptive local binarization method for document images based on a novel thresholding method and dynamic windows", 2011. (unreproducible)
#' \item ISauvola - "ISauvola: Improved Sauvola’s Algorithm for Document Image Binarization", 2016.
#' \item WAN - "Binarization of Document Image Using Optimum Threshold Modification", 2018.
#' }
#' @param x an image of class 'magick-image'. In grayscale. E.g. a PGM file. If not provided in grayscale, will extract the gray channel.
#' @param type a character string with the type of binarization to use. Either 'otsu', 'bernsen', 'niblack', 'sauvola', 'wolf', 'nick', 'gatos', 'su', 'trsingh', 'bataineh', 'wan' or 'isauvola'
#' @param opts a list of options to pass on to the algorithm. See the details and the examples.
#' @details 
#' Options which can be bassed on to the binarization routines, with the defaults between brackets
#' \itemize{
#' \item otsu: none
#' \item bernsen: window(75L), k(0.2), threshold(100L), contrast-limit(25L)
#' \item niblack: window(75L), k(0.2)
#' \item sauvola: window(75L), k(0.2)
#' \item wolf: window(75L), k(0.2)
#' \item nick: window(75L), k(-0.2)
#' \item gatos: window(75L), k(0.2), glyph(60L)
#' \item su: window(75L), minN(75L)
#' \item trsingh: window(75L), k(0.2)
#' \item bataineh: none
#' \item wan: window(75L), k(0.2)
#' \item isauvola: window(75L), k(0.2)
#' }
#' Note that it is important that you provide the window / threshold / contrast-limit, minN, glyph argument as integers (e.g. as in 75L)
#' and the other parameters as numerics.
#' @export 
#' @return a binarized image of class magick-image as handled by the magick R package
#' @examples 
#' library(magick)
#' f   <- system.file("extdata", "doxa-example.png", package = "image.binarization")
#' img <- image_read(f)
#' img <- image_convert(img, format = "PGM", colorspace = "Gray")
#' 
#' binary <- image_binarization(img, type = "otsu")
#' binary
#' binary <- image_binarization(img, type = "bernsen", 
#'                              opts = list(window = 50L, k = 0.2, threshold = 50L))
#' binary
#' binary <- image_binarization(img, type = "niblack", opts = list(window = 75L, k = 0.2))
#' binary
#' binary <- image_binarization(img, type = "sauvola")
#' binary
#' binary <- image_binarization(img, type = "wolf")
#' binary
#' binary <- image_binarization(img, type = "nick", opts = list(window = 75L, k = -0.2))
#' binary
#' binary <- image_binarization(img, type = "gatos", opts = list(window = 75L, k = 0.2, glyph = 50L))
#' binary
#' binary <- image_binarization(img, type = "su", opts = list(window = 20L))
#' binary
#' binary <- image_binarization(img, type = "trsingh")
#' binary
#' binary <- image_binarization(img, type = "bataineh")
#' binary
#' binary <- image_binarization(img, type = "wan")
#' binary
#' binary <- image_binarization(img, type = "isauvola", opts = list(window = 75L, k = 0.2))
#' binary
image_binarization <- function(x, type, opts = list()){
    stopifnot(type %in% c("otsu", "bernsen", "niblack", "sauvola", "wolf", "nick", "gatos", "su", "trsingh", "bataineh", "wan", "isauvola"))
    i <- magick::image_info(x)
    if(nrow(i) > 1){
        stop("works only on magick objects containing 1 image")
    }
    width  <- i$width
    height <- i$height
    info   <- list(x = as.vector(magick::image_data(x, channel = "gray"), mode = 'integer'), width = width, height = height)
    ## Apply
    binary <- doxa_binary(info$x, width = info$width, height = info$height, type = type, opts = opts)
    ## Get the results back into magick
    img <- matrix(binary$x, ncol = binary$width, nrow = binary$height, byrow = TRUE)
    img <- grDevices::as.raster(img, max = 255)
    img <- magick::image_read(img)
    img
}
