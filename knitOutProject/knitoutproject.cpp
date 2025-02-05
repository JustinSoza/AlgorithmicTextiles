#include <iostream>
#include <fstream>
#include <string>
#include "fssimplewindow.h"
#include "yspng.h"
#include "yspngenc.h"


const int carrierA = 1;
const int carrierB = 5;

std::string fileName = "output.txt";
std::ofstream file {fileName};

// Initialize highlight color (BLACK PIXELS)
void init1(int wid, int carrier)
{
    file << ";!knitout-2\n";
    file << ";;Carriers: 1 2 3 4 5 6 7 8 9 10\n\n";
    file << ";; Position: Center" << "\n";
    file << "; bottom row\n\n";
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

void clear(int width, int A, int B)
{
    for(int i=1; i<=width; ++i) // Finish A
    {
        file << "knit + f" << i << " " << A << "\n";
    }
    file << "outhook " << A << "\n";

    for(int i=1; i<=width; ++i) // Finish B
    {
        file << "knit + f" << i << " " << B << "\n";
    }
    file << "outhook " << B << "\n";

    for(int i=1; i<=width; ++i) // Clear hooks
    {
        file << "drop f" << i << "\n";
    }
}

int main()
{
    FsChangeToProgramDir();
    YsRawPngDecoder png;

    char dir[200] = "../../knitOutProject/";
    char imagename[200];
    std::cout << "Enter bitmap file name: ";
	std::cin  >> imagename;
	std::cout << "Loading " << imagename << "...\n";

	if (YSOK == png.Decode(imagename))
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
    clear(width, carrierA, carrierB);

    std::cout << "\n\nKnitout saved to " << fileName;
}