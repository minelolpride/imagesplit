#include <iostream>
#include "bitmap.h"

using namespace std;

int main(void) {
	string inputName;					// input filename
	string modeIn;						// the cutting orientation
	unsigned int inImgCurY = 0;			// where are we on the original image
	unsigned int inImgCurX = 0;			//
	unsigned int outImgCurY = 0;		// where are we on the output canvas
	unsigned int outImgCurX = 0;		//
	unsigned int splitSize = 0;			// how big each split should be
	bool splitVert;						// if true, the cuts are vertical, otherwise they are horizontal
	bool splitSet = false;				// a failsafe for later

	// take in the input filename
	cout << "What is your input image filename? (Excluding extension) (Must be BMP!)" << endl;
	getline(cin, inputName); // we can use files with spaces in the name
	string inputName2 = inputName + ".bmp";
	bitmap_image inputImage(inputName2);
	if (!inputImage) {
		cout << "Could not fetch that image.";
		return 1;
	}

	// tells the image dimensions
	cout << "Image is " << inputImage.width() << "x" << inputImage.height() << endl << endl;

	// get the cutting orientation
	cout << "How would you like this image to be split? ( H / V )" << endl;
	getline(cin, modeIn);
	if (modeIn == "H" || modeIn == "h") {
		splitSet = true;
		splitVert = false;
	}
	if (modeIn == "V" || modeIn == "v") {
			splitSet = true;
			splitVert = true;
	}
	if (!splitSet) return 1; // there it is

	// get the desired split size
	cout << "How ";
	if (splitVert) cout << "wide "; else cout << "tall ";
	cout << "should each split be? (in pixels)" << endl;
	cin >> splitSize;

	// just say something so we know its working
	cout << "Processing image...";

	// work the magic
	if (splitVert) {
		unsigned int splitCount = inputImage.width() / splitSize;
		splitCount++;
		cout << splitCount << endl;
		unsigned int i = 1;
		while (i <= splitCount) {
			bitmap_image outputSplit(splitSize, inputImage.height());
			outputSplit.clear(0xFF);
			while (outImgCurX < splitSize) {
				while (outImgCurY <= inputImage.height() - 1) {
					// following line existed to help debugging, uncomment if you want to see the program work (warning: a bit slow)
					//cout << "Pixel at " << outImgCurX << " " << outImgCurY << " will be " << (int)inputImage.get_pixel(inImgCurX, inImgCurY).red << " " << (int)inputImage.get_pixel(inImgCurX, inImgCurY).green << " " << (int)inputImage.get_pixel(inImgCurX, inImgCurY).blue << endl;
					outputSplit.set_pixel(outImgCurX, outImgCurY, inputImage.get_pixel(inImgCurX, inImgCurY).red, inputImage.get_pixel(inImgCurX, inImgCurY).green, inputImage.get_pixel(inImgCurX, inImgCurY).blue);
					inImgCurY++;
					outImgCurY++;
				}
				inImgCurY = 0;
				outImgCurY = 0;
				inImgCurX++;
				outImgCurX++;
			}
			outputSplit.save_image(inputName + "_" + to_string(i) + ".bmp");
			outImgCurX = 0; outImgCurY = 0;
			i++;
		}
		cout << "done." << endl;
	} else {
		unsigned int splitCount = inputImage.height() / splitSize; splitCount++;
		unsigned int i = 1;
		while (i <= splitCount) {
			bitmap_image outputSplit(inputImage.width(), splitSize);
			outputSplit.clear(0xFF);
			while (outImgCurY < splitSize) {
				while (outImgCurX <= inputImage.width() - 1) {
					// following line existed to help debugging, uncomment if you want to see the program work (warning: a bit slow)
					//cout << "Pixel at " << outImgCurX << " " << outImgCurY << " will be " << (int)inputImage.get_pixel(inImgCurX, inImgCurY).red << " " << (int)inputImage.get_pixel(inImgCurX, inImgCurY).green << " " << (int)inputImage.get_pixel(inImgCurX, inImgCurY).blue << endl;
					outputSplit.set_pixel(outImgCurX, outImgCurY, inputImage.get_pixel(inImgCurX, inImgCurY).red, inputImage.get_pixel(inImgCurX, inImgCurY).green, inputImage.get_pixel(inImgCurX, inImgCurY).blue);
					inImgCurX++;
					outImgCurX++;
				}
				inImgCurX = 0;
				outImgCurX = 0;
				inImgCurY++;
				outImgCurY++;
			}
			outputSplit.save_image(inputName + "_" + to_string(i) + ".bmp");
			outImgCurX = 0; outImgCurY = 0;
			i++;
		}
		cout << "done." << endl;
	}
	return 0;
}