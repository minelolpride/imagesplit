#include <iostream>
#include "bitmap.h"

// one limitation of working with this header for processing BMP files
// is that it only works with 24bpp images, meaning we cannot utilize
// the alpha channel when splitting. it would be useful to have that
// functionality but i currently will work with what i have right now.
// this also means that some splits will have the default white
// canvas peek through, and that is because the image is not just not
// the right size to split evenly into chunks of whatever size you
// desire. so for now this is how it will be.

using namespace std;

int main(void) {
	string inputName;					// input filename
	string modeIn;						// the cutting orientation input
	unsigned int inImgCurY = 0;			// where are we on the original image
	unsigned int inImgCurX = 0;			//
	unsigned int outImgCurY = 0;		// where are we on the output canvas
	unsigned int outImgCurX = 0;		//
	double splitSize = 0.0;				// how big each split should be
	double splitSize2 = 0.0;			// this one is used in the grid
	bool splitSet = false;				// a failsafe for later
	int splitMode;						// how should we split the image?
										// 0 = vertically
										// 1 = horizontally
										// 2 = grid

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
	cout << "How would you like this image to be split?" << endl << "[H]orizontal, [V]ertical, [G]rid" << endl;
	cin >> modeIn;
	if (modeIn == "V" || modeIn == "v") {
		splitSet = true;
		splitMode = 0;
	}
	if (modeIn == "H" || modeIn == "h") {
		splitSet = true;
		splitMode = 1;
	}
	if (modeIn == "G" || modeIn == "g") {
		splitSet = true;
		splitMode = 2;
	}
	if (!splitSet) return 1; // there it is

	// enter here if we are cutting vertically
	if (splitMode == 0) {
		cout << "How wide should each split be? (in pixels) ";
		cin >> splitSize;
		cout << "Processing image...";
		unsigned int splitCount = ceil(inputImage.width() / splitSize);
		unsigned int i = 1;
		while (i <= splitCount) {
			bitmap_image outputSplit(splitSize, inputImage.height());
			outputSplit.clear(0xFF);
			while (outImgCurX < splitSize) {
				while (outImgCurY <= inputImage.height() - 1) {
					if (inImgCurX < inputImage.width() && inImgCurY < inputImage.height()) {
						outputSplit.set_pixel(outImgCurX, outImgCurY, inputImage.get_pixel(inImgCurX, inImgCurY).red, inputImage.get_pixel(inImgCurX, inImgCurY).green, inputImage.get_pixel(inImgCurX, inImgCurY).blue);
					}
					inImgCurY++;
					outImgCurY++;
				}
				inImgCurY = 0;
				outImgCurY = 0;
				inImgCurX++;
				outImgCurX++;
			}
			outputSplit.save_image(inputName + "_" + to_string(i) + ".bmp");
			outImgCurX = 0;
			i++;
		}
		cout << "done." << endl;
	}

	// enter here if we are cutting horizontally
	if (splitMode == 1) {
		cout << "How tall should each split be? (in pixels) ";
		cin >> splitSize;
		cout << "Processing image...";
		unsigned int splitCount = ceil(inputImage.height() / splitSize);
		unsigned int i = 1;
		while (i <= splitCount) {
			bitmap_image outputSplit(inputImage.width(), splitSize);
			outputSplit.clear(0xFF);
			while (outImgCurY < splitSize) {
				while (outImgCurX <= inputImage.width() - 1) {
					if (inImgCurX < inputImage.width() && inImgCurY < inputImage.height()) {
						outputSplit.set_pixel(outImgCurX, outImgCurY, inputImage.get_pixel(inImgCurX, inImgCurY).red, inputImage.get_pixel(inImgCurX, inImgCurY).green, inputImage.get_pixel(inImgCurX, inImgCurY).blue);
					}
					inImgCurX++;
					outImgCurX++;
				}
				inImgCurX = 0;
				outImgCurX = 0;
				inImgCurY++;
				outImgCurY++;
			}
			outputSplit.save_image(inputName + "_" + to_string(i) + ".bmp");
			outImgCurY = 0;
			i++;
		}
		cout << "done." << endl;
	}
	
	// enter here if we are cutting into grid pieces
	if (splitMode == 2) {
		cout << "How wide should each split be? (in pixels) ";
		cin >> splitSize;
		cout << "How tall should each split be? (in pixels) ";
		cin >> splitSize2;

		unsigned int splitsX = ceil(inputImage.width() / splitSize);
		unsigned int splitsY = ceil(inputImage.height() / splitSize2);
		unsigned int splitCount = splitsX + splitsY;

		unsigned int x = 1; // keep track of what split we are on
		unsigned int y = 1;

		while (y <= splitsY) {
			while (x <= splitsX) {
				bitmap_image outputSplit(splitSize, splitSize2);
				outputSplit.clear(0xFF);
				while (outImgCurY < splitSize2) {
					while (outImgCurX < splitSize) {
						//cout << inImgCurX << " " << inImgCurY << endl;
						if (inImgCurX < inputImage.width() && inImgCurY < inputImage.height()) {
							outputSplit.set_pixel(outImgCurX, outImgCurY, inputImage.get_pixel(inImgCurX, inImgCurY).red, inputImage.get_pixel(inImgCurX, inImgCurY).green, inputImage.get_pixel(inImgCurX, inImgCurY).blue);
						}
						outImgCurX++;
						inImgCurX++;
					}
					outImgCurY++;
					outImgCurX = 0;
					inImgCurX = (splitSize * x) - splitSize;
					inImgCurY++;
				}
				outImgCurY = 0;
				inImgCurY = (splitSize2 * y) - splitSize2;
				outputSplit.save_image(inputName + "_" + to_string(x) + "_" + to_string(y) + ".bmp");
				x++;
			}
			x = 1;
			y++;
			inImgCurX = 0;
			inImgCurY = (splitSize2 * y) - splitSize2;
		}
	}
	return 0;
}
