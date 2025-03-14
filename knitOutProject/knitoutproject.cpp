#include <iostream>
#include <fstream>
#include <string>
#include "fssimplewindow.h"
#include "yspng.h"
#include "yspngenc.h"

// Carrier A (Black Pixels), Carrier B (White Pixels)
const int carrierA = 3;
const int carrierB = 4;

// Image folder directory
char folderDir[200] = "../../knitOutProject/images/";

// Output file
std::string fileName = "output.k";
std::ofstream file {fileName};

// Initialize highlight color (BLACK PIXELS)
void init1(int wid, int carrier)
{
    file << ";!knitout-2\n";
    file << ";;Carriers: 1 2 3 4 5 6 7 8 9 10\n\n";
    file << ";; Position: Center" << "\n";
    file << "; bottom row\n\n";
    file << "; Carrier 3: Black\n";
    file << "; Carrier 4: White\n\n";
    file << "inhook " << carrier << "; Initialize carrier A" << "\n\n";
    
    for(int i=wid; i>0; --i) // Tuck left
    {
        if (i%2==0)
        {
            file << "tuck - f" << i << " " << carrier << "\n";
        }
    }

    for(int i=1; i<=wid; ++i) // Tuck right
    {
        if (i%2!=0)
        {
            file << "tuck + f" << i << " " << carrier << "\n";
        }
    }

    for(int i=wid; i>1; --i) // Knit left row
    {
        file << "knit - f" << i << " " << carrier << "\n";
    }

    for(int i=1; i<=wid; ++i) // Knit right row
    {
        file << "knit + f" << i << " " << carrier << "\n";
    }
    file << "releasehook " << carrier << "\n";

    for(int i=wid; i>0; --i) // Knit left row
    {
        file << "knit - f" << i << " " << carrier << "\n";
    }
    file << "\n";
}

// Initialize background color (WHITE PIXELS)
void init2(int wid, int carrier)
{
    file << "inhook " << carrier << "; Initialize carrier B" << "\n\n";
    for(int i=wid; i>1; --i) // Knit left row
    {
        file << "knit - f" << i << " " << carrier << "\n";
    }

    for(int i=1; i<=wid; ++i) // Knit right row
    {
        file << "knit + f" << i << " " << carrier << "\n";
    }
    file << "releasehook " << carrier << "\n";

    for(int i=wid; i>0; --i) // Knit left row
    {
        file << "knit - f" << i << " " << carrier << "\n";
    }
    file << "\n";
}

// Creates pattern from binary map
void knitFromBitmap(int width, int A, int B, YsRawPngDecoder& img)
{
    bool black = true;
    file << "; black pixels: " << black << "\n\n";
    for (int i = 0; i < img.hei; ++i)
	{
        // For debugging
        std::cout << "\n";
        file << "; First pass [Moving right], tuck color behind\n";
        // for (int j = 0; j < width; ++j) // First pass TUCKS
        // {
        //     auto r = img.rgba[(i*width + j) * 4];
		//     auto g = img.rgba[(i*width + j) * 4 + 1];
		//     auto b = img.rgba[(i*width + j) * 4 + 2];
        //     int bit = (r+g+b)/3;

        //     if (!black && bit < 100 && (j+i)%6==0) // knit black pixels A
        //     {
        //         file << "tuck + f" << j+1 << " " << A << "\n";
        //     }
        //     else if (black && bit > 100 && (j+i)%6==0) //knit white pixels B
        //     {
        //         file << "tuck + f" << j+1 << " " << B << "\n";
        //     }
        // }

        file << "; First pass [Moving right], knit color in front \n";
        int blackCount = 0;
        for (int j = 0; j < width; ++j) // First pass
        {
            auto r = img.rgba[(i*width + j) * 4];
		    auto g = img.rgba[(i*width + j) * 4 + 1];
		    auto b = img.rgba[(i*width + j) * 4 + 2];
            int bit = (r+g+b)/3;
            if (bit < 100){++blackCount;}

            if (black && bit < 100) // knit black pixels A
            {
                file << "knit + f" << j+1 << " " << A << "\n";
            }
            else if (black && bit > 100 && (j+i)%6==0) //knit white pixels B
            {
                file << "tuck + f" << j+1 << " " << A << "\n";
            }
            else if (!black && bit > 100) //knit white pixels B
            {
                file << "knit + f" << j+1 << " " << B << "\n";
            }
            else if (!black && bit < 100 && (j+i)%6==0) // knit black pixels A
            {
                file << "tuck + f" << j+1 << " " << B << "\n";
            }
            else if  (black && j==width-1) // Check for miss
            {   
                if (blackCount > 0){file << "miss + f" << width << " " << A << "\n";}
            }

            // For debugging
            if (bit > 100){std::cout << "-";}
            else          {std::cout << "X";}
        }

        file << "; Transfer row to back\n";
        for (int j = 0; j < width; ++j) // First xfer 
        {
            auto r = img.rgba[(i*width + j) * 4];
		    auto g = img.rgba[(i*width + j) * 4 + 1];
		    auto b = img.rgba[(i*width + j) * 4 + 2];
            int bit = (r+g+b)/3;

            if (black && bit < 100) // knit black pixels A
            {
                file << "xfer f" << j+1 << " b" << j+1 << "\n";
            }
            else if (!black && bit > 100) //knit white pixels B
            {
                file << "xfer f" << j+1 << " b" << j+1 << "\n";
            }

        }

        // file << "; Second pass [Moving left], tuck color in back\n";
        // for (int j = width-1; j >= 0; --j) // Second pass TUCKS
        // {
        //     auto r = img.rgba[(i*width + j) * 4];
        //     auto g = img.rgba[(i*width + j) * 4 + 1];
        //     auto b = img.rgba[(i*width + j) * 4 + 2];
        //     int bit = (r+g+b)/3;

        //     if (!black && bit < 100 && (j+i)%6==0) // knit black pixels A
        //     {
        //         file << "tuck - b" << j+1 << " " << A << "\n";
        //     }
        //     else if (black && bit > 100 && (j+i)%6==0) //knit white pixels B
        //     {
        //         file << "tuck - b" << j+1 << " " << B << "\n";
        //     }
        // }

        file << "; Second pass [Moving left], knit color in front\n";
        blackCount = 0;
        for (int j = width-1; j >= 0; --j) // Second pass
        {
            auto r = img.rgba[(i*width + j) * 4];
            auto g = img.rgba[(i*width + j) * 4 + 1];
            auto b = img.rgba[(i*width + j) * 4 + 2];
            int bit = (r+g+b)/3;
            if (bit < 100){++blackCount;}

            if (black && bit < 100) // knit black pixels A
            {
                file << "knit - b" << j+1 << " " << A << "\n";
            }
            else if (black && bit > 100 && (j+i)%6==0) //knit white pixels B
            {
                file << "tuck - b" << j+1 << " " << A << "\n";
            }
            else if (!black && bit > 100) //knit white pixels B
            {
                file << "knit - b" << j+1 << " " << B << "\n";
            }
            else if (!black && bit < 100 && (j+i)%6==0) // knit black pixels A
            {
                file << "tuck - b" << j+1 << " " << B << "\n";
            }
            else if  (black && j==0) // Check for miss
            {   
                if (blackCount > 0){file << "miss - b" << 1 << " " << A << "\n";}
            }
        }


        file << "; Transfer row back to front\n";
        for (int j = width-1; j >= 0; --j) // Second xfer
        {
            auto r = img.rgba[(i*width + j) * 4];
            auto g = img.rgba[(i*width + j) * 4 + 1];
            auto b = img.rgba[(i*width + j) * 4 + 2];
            int bit = (r+g+b)/3;

            if (black && bit < 100) // knit black pixels A
            {
                file << "xfer b" << j+1 << " f" << j+1 << "\n";
            }
            else if (!black && bit > 100) //knit white pixels B
            {
                file << "xfer b" << j+1 << " f" << j+1 << "\n";
            }
        }

        black = !black;
        // for(pixel in row)
        // knit black pixel carrier A
        // miss width
        // xfer black pixels
        // change direction
        // knit black pixel carrier A
        // miss width
        // xfer black pixels
        // 
        // knit white pixel carrier B
        // xfer white pixels
        // change direction
        // knit white pixels carrier B
        // xfer white pixels
        //
        // knit black pixles carrier A
        // miss width
        // xfer black pixels
        // change direction
        // knit black pixels carrier A
        // miss width
        // xfer black pixels
        // 
        // knit white pixels carrer B
        // xfer white pixles
        // change direction
        // knit white pixels carrier B
        // xfer white pixels
	}
}

void bindoff(int width, int A, int B)
{
    for(int i=1; i<=width; ++i){file << "knit + f" << i << " " << A << "\n";}
    for(int i=width; i>0; --i){file << "knit - f" << i << " " << A << "\n";}

    file << "outhook " << A << "\n";

    // Transfer back to front, then knit
    for(int i=1; i<=width; ++i){file << "xfer b" << i << " f" << i << "\n";}
    for(int i=1; i<=width; ++i){file << "knit + f" << i << " " << B << "\n";}
    for(int i=width; i>0; --i){file << "knit - f" << i << " " << B << "\n";}

    file << "\n; Bindoff\n";
    for(int i=1; i<=width-1; ++i)
    {
        file << "xfer f" << i << " b" << i << "\n";
        file << "rack 1.0\n";
        file << "xfer b" << i << " f" << i+1 << "\n";
        file << "rack 0.25\n";
        
        if(i%2==0){file << "tuck + b" << i << " " << B << "\n";}
        file << "knit + f" << i+1 << " " << B << "\n";
        if(i<width-1){file << "miss + f" << i+2 << " " << B << "\n";}

        file << "rack 0.0\n";
    }

    file << "\n; Overhang\n";
    file << "knit - f" << width << " " << B << "\n";
    file << "knit + f" << width << " " << B << "\n";
    file << "knit - f" << width << " " << B << "\n";
    file << "knit + f" << width << " " << B << "\n";
    file << "outhook " << B << "\n";
    file << "rack 0.25\n";

    file << "\n ; Drop hooks\n";
    for(int i=1; i<=width; ++i) // Clear hooks
    {
        file << "drop f" << i << "\n";
        file << "drop b" << i << "\n";
    }
}

int main()
{
    FsChangeToProgramDir();
    YsRawPngDecoder png;

    char fileName[200];
    char imageDir[400];

    std::cout << "In directory: " << folderDir << "\n";
    std::cout << "Enter image file name: ";
	std::cin  >> fileName;
	std::cout << "Loading " << fileName << "...\n";

    std::strcpy(imageDir, folderDir);
    std::strcat(imageDir, fileName);
	if (YSOK == png.Decode(imageDir))
	{
		printf("Wid %d Hei %d\n", png.wid, png.hei);
	}
    else
	{
		printf("Failed to open file.\n");
		return 1;
	}

    int width = png.wid;
    int length = png.hei;
    png.Flip();

    init1(width, carrierA);
    init2(width, carrierB);
    knitFromBitmap(width, carrierA, carrierB, png);
    bindoff(width, carrierA, carrierB);

    std::cout << "\n\nKnitout saved to " << fileName;
    return 0;
}