#include <vector>
#include <iostream>
#include <fstream>
#include <math.h>
#include <unistd.h>

#define WIDTH 48
#define HEIGHT 24

class particle
{
    public:
    bool move(char,int,int,std::vector<std::vector<particle>>&);

    int type = 0;
    bool updated = false;
};

bool particle::move(char dir, int x, int y, std::vector<std::vector<particle>>& world)
{
    if(this->type == 0 || this->type == 1)
    {
        return false;
    }

    if(dir == 'v')
    {
        particle& target = world.at(y+1).at(x);
        if(target.type == 0)
        {
            int t = this->type;
            target.type = t;
            target.updated = true;
            this->type = 0;
            this->updated = true;
            return true;
        }
        else if(target.type == 2)
        {
            if(target.move('v',x,y+1,world))
            {
                int t = this->type;
                target.type = t;
                target.updated = true;
                this->type = 0;
                this->updated = true;
                return true;
            }
        }
        // Try left
        if(this->move('<',x,y,world))
        {
            return false;
        }
        // Try right
        else if(this->move('>',x,y,world))
        {
            return true;
        }
        else if(this->move('^',x,y-1,world))
        {
            return false;
        }
    }
    else if(dir == '^')
    {
        // uwu, can't go up I guess
        return false;
    }
    else if(dir == '<')
    {
        particle& target = world.at(y).at(x-1);
        if(target.type == 0)
        {
            int t = this->type;
            target.type = t;
            target.updated = true;
            this->type = 0;
            this->updated = true;
            return true;
        }
        else if(target.type == 2)
        {
            if(target.move('<',x,y+1,world))
            {
                int t = this->type;
                target.type = t;
                target.updated = true;
                this->type = 0;
                this->updated = true;
                return true;
            }
        }
    }
    else if(dir == '>')
    {
        particle& target = world.at(y).at(x+1);
        if(target.type == 0)
        {
            int t = this->type;
            target.type = t;
            target.updated = true;
            this->type = 0;
            this->updated = true;
            return true;
        }
        else if(target.type == 2)
        {
            if(target.move('>',x,y+1,world))
            {
                int t = this->type;
                target.type = t;
                target.updated = true;
                this->type = 0;
                this->updated = true;
                return true;
            }
        }
    }
    return false;
}

int main()
{
    // Init
    std::cout<<"\x1b[2J";
    std::vector<std::vector<particle>> particles;
    std::string separator;
    {
        std::vector<particle> line;
        for(int x = 0;x<WIDTH;x++)
        {
            particle p;
            p.type = 1;
        }
        for(int x = 0; x < WIDTH; x++)
        {
            particle p;
            line.push_back(p);
        }
        for(int y = 0; y < HEIGHT; y++)
        {
            particles.push_back(line);
        }
    }

    // Placing stuff
    {
        std::ifstream f("sim.txt");
        if(!f){
            std::cout<<"bruh moment\n";
            return -1;
        }
        std::string line;
        int y = 0;
        while(std::getline(f,line))
        {
            int x = 0;
            for(char c : line)
            {
                int t = 0;
                if(c == '#')
                {
                    t = 1;
                }
                else if(c == 'o')
                {
                    t = 2;
                }
                particles.at(y).at(x).type = t;
                x++;
            }
            y++;
        }
        f.close();
    }

    // Update
    while(true)
    {
        std::cout<<"\x1b[1;1H";

        for(int y = 0; y < HEIGHT; y++)
        {
            for(int x = 0; x < WIDTH; x++)
            {
                if(!particles.at(y).at(x).updated && particles.at(y).at(x).type == 2)
                {
                    particles.at(y).at(x).move(
                        'v',
                        x,
                        y,
                        particles
                    );
                }
            }
        }

        for(int y = 0; y < HEIGHT; y++)
        {
            std::string l;
            for(int x = 0; x < WIDTH; x++)
            {
                int t = particles.at(y).at(x).type;

                if(t == 0)
                {
                    l.push_back(' ');
                }
                else if(t==1)
                {
                    l.push_back('#');
                }
                else if(t==2)
                {
                    l.push_back('o');
                }

                particles.at(y).at(x).updated = false;
            }
            std::cout<<l<<"\n";
        }
        usleep(100000);
    }


    return 0;
}