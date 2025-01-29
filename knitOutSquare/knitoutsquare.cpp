#include <iostream>
#include <fstream>

std::ofstream file {"knitoutText.txt"};

void init(int wid)
{
    file << ";!knitout-2\n";
    file << ";;Carriers: 1 2 3 4 5 6 7 8 9 10\n\n";
    file << "; bottom row\n\n";
    file << "inhook " << wid << "\n\n";
    for(int i=1;i<=wid;++i)
    {
        if (i%2 != 0) // even tucks
        {
            file << "tuck";
            file << " + f" << i << " " << wid << "\n"; 
        }
    }

    for(int i=wid;i>0;--i)
    {
        
        if (i%2 == 0) // odd tucks
        {
            file << "tuck";
            file << " - b" << i << " " << wid << "\n"; 
        }
    }

    for(int i=1;i<=wid;++i) // cross stitch
    {
        file << "knit + ";
        if (i%2 == 0)
        {
            file << "b" << i << " " << wid << "\n"; 
        }
        if (i%2 != 0)
        {
            file << "f" << i << " " << wid << "\n"; 
        }
    }
    file << "\n" << "releasehook " << wid << "\n\n";
}

void knit(int wid, int len)
{
    bool backward = true;
    for(int j=0;j<len;++j)
    {
        if (backward)
        {
            for(int i=wid;i>0;--i)
            {
                file << "knit - ";
                if (i%2 == 0)
                {
                    file << "b" << i << " " << wid << "\n"; 
                }
                if (i%2 != 0)
                {
                    file << "f" << i << " " << wid << "\n"; 
                }
            }
            file<<"\n";
            backward = false;
        }
        else
        {
            for(int i=1;i<=wid;++i)
            {
                file << "knit + ";
                if (i%2 == 0)
                {
                    file << "b" << i << " " << wid << "\n"; 
                }
                if (i%2 != 0)
                {
                    file << "f" << i << " " << wid << "\n"; 
                }
            }
            file<<"\n";
            backward = true;
        }
    }
}

void clear(int wid)
{
    file << "outhook " << wid << "\n\n";
    for(int i=1;i<=wid;++i)
    {
        file << "drop ";
        if (i%2 == 0)
        {
            file << "b" << i << "\n"; 
        }
        if (i%2 != 0)
        {
            file << "f" << i << "\n"; 
        }
    }
}

int main()
{
    int width;
    int passes;
    std::cout << "Enter Width: ";
    std::cin >> width;
    std::cout << "Enter Length: ";
    std::cin >> passes;
    init(width);
    knit(width, passes);
    clear(width);
    return 0;
}