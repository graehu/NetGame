#ifndef RENDERER_H
#define RENDERER_H


class Renderer
{
    public:
        Renderer();
        virtual ~Renderer();
        bool beginScene(); //bool* keys
        void draw(float xPos, float yPos, float xDir, float yDir);
        void endScene();
    protected:
    private:
        unsigned int xRes, yRes;
};

#endif // RENDERER_H
