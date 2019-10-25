#ifndef SHADER_H
#define SHADER_H

class Shader {

    public:

    Shader(const char* vertPath, const char* fragPath);
    ~Shader();

    unsigned int getID();
    void setID(unsigned int ID);

    private:

    unsigned int ID;
};

#endif