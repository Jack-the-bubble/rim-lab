#define DIM 1000 /* rozmiar rysunku w pikselach */

#include <math.h>
#include <GL/freeglut.h>

float c_rel = -0.123f;
float c_im = 0.745f;
float c_abs = (float) sqrt(0.5*0.5*2);
float R2;
const int k = 100;

int julia(float x, float y)
{

    int i = 0;
    float x2 = x*x,
          y2 = y*y;

    while (i < k)
    {
        if (x2 + y2 > R2)
            return 0;
        
        y = 2*x*y + c_im;
        x = x2 - y2 + c_rel;
        i++;
        x2 = x*x;
        y2 = y*y;
    }
    return 1;
}

void kernel (unsigned char *ptr,
             const int xw, const int yw,
             const float dx, const float dy,
             const float scale)
{
    // przeliczenie współrzędnych pikselowych na matematyczne 
    // z uwzględnieniem skali (skale) i matematycznego środka (dx, dy)
    float x = scale * (float) (xw-DIM/2)/(DIM/2) + dx,
          y = scale * (float) (yw-DIM/2)/(DIM/2) + dy;

    // offset w buforze pikseli
    int offset = xw + yw*DIM;
    // kolor czarny dla uciekinierów (julia == 0) 
    // i czerwony dla więźniów (julia == 1)

    ptr[offset*4 + 0] = (unsigned char) (255*julia(x, y)); //R
    ptr[offset*4 + 1] = 0; //G
    ptr[offset*4 + 2] = 0; //B
    ptr[offset*4 + 3] = 255; //A
}

static unsigned char pixbuf[DIM * DIM * 4];
static float dx = 0.0f, dy = 0.0f;
static float scale = 1.5f;

static void disp(void)
{
    glDrawPixels(DIM, DIM, GL_RGBA, GL_UNSIGNED_BYTE, pixbuf);
    glutSwapBuffers();
}

static void recompute(void)
{
    int xw, yw;
    for (yw = 0; yw < DIM; yw++)
        for (xw = 0; xw < DIM; xw++)
            kernel(pixbuf, xw, yw, dx, dy, scale);
    glutPostRedisplay();
}

static void kbd(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'p': 
        dx += scale*(float) (x - DIM/2)/(DIM/2);
        dy -= scale*(float) (y - DIM/2)/(DIM/2);
        break;
    case 'z': 
        scale *= 0.80f;
        break;
    case 'Z':
        scale *= 1.25f;
        break;
    case '=':
        scale *= 1.50f;
        dx = dy = 0.0f;
        break;
    case 27: //Esc
        exit(0);
    default:
        break;
    }
    recompute();
}

int main(int argc, char *argv[])
{
    R2 = (c_abs > 2 ? c_abs : 2) * (c_abs > 2 ? c_abs : 2);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(DIM, DIM);
    glutCreateWindow("RIM - fraktal Julii");
    glutDisplayFunc(disp);
    glutKeyboardFunc(kbd);
    recompute();
    glutMainLoop();
}