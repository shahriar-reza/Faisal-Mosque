#include <bits/stdc++.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include "glut.h"

using namespace std;

#define pi (2*acos(0.0))
double R, Rspeed, vAngle, hAngle, angleSpeed;
double up_down, left_right, rotate_x, rotate_y;
double eyeX = 0, eyeY = 0, eyeZ = 0, centerX = 0, centerY = 1, centerZ = 0, upX, upY, upZ;
double prev_mouse_x = 0;

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle, rotation;

int num_texture = 0;
GLuint wallImage, MarbleImage, skyImage;

double small_ = 20.0, big = 20.0;


struct point
{
    double x,y,z;
};


void drawAxis(){
    glBegin(GL_LINES);

	//+X axis
	glColor3f(1, 0, 0);//red
	glVertex3f(0, 0, 0);
	glVertex3f(1000, 0, 0);

	//-X axis
	glColor3f(1, 0, 1);//purple
	glVertex3f(0, 0, 0);
	glVertex3f(-1000, 0, 0);

    //+Y axis
	glColor3f(0, 1, 0);//green
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1000, 0);

	//-Y axis
	glColor3f(1, 1, 0);//yellow
	glVertex3f(0, 0, 0);
	glVertex3f(0, -1000, 0);

	//+Z axis
	glColor3f(0, 0, 1);//blue
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1000);

	//-Z axis
	glColor3f(0, 1, 1);//cyan
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, -1000);
	glEnd();
}


void drawGrid(){
    int i;

	glColor3f(0.5, 0.5, 0.5);//gray

	glBegin(GL_LINES);
	for (i = -100; i <= 100; i++)
	{
		if (i == 0)
		{
			continue;//SKIP the MAIN axes
		}

		//lines parallel to Y-axis
		glVertex3f(i * 10, -1000, 0);
		glVertex3f(i * 10, 1000, 0);

		//lines parallel to X-axis
		glVertex3f(-1000, i * 10, 0);
		glVertex3f(1000, i * 10, 0);
	}
	glEnd();
}


int loadBitmapImage(string ff)
{
    printf("line 1\n");
	int i, j = 0;
	FILE *l_file;
	BYTE *l_texture;

	BITMAPFILEHEADER fileheader;
	BITMAPINFOHEADER infoheader;
	RGBTRIPLE rgb;

	num_texture++;

    char filename[103];
    for(int i=0;i<ff.size();++i){
        filename[i]=ff[i];
    }
    filename[ff.size()]='\0';

	if ((l_file = fopen(filename, "rb")) == NULL)
	{
        cout<<"null\n";
		return (-1);
	}
    cout<<(int)infoheader.biWidth<<"\n";
    cout<<(int)infoheader.biHeight<<"\n";
	fread(&fileheader, sizeof(fileheader), 1, l_file);

	fseek(l_file, sizeof(fileheader), SEEK_SET);
	fread(&infoheader, sizeof(infoheader), 1, l_file);

	l_texture = (BYTE *)malloc(infoheader.biWidth * infoheader.biHeight * 4);
	memset(l_texture, 0, infoheader.biWidth * infoheader.biHeight * 4);
	for (i = 0; i < infoheader.biWidth * infoheader.biHeight; i++)
	{
		fread(&rgb, sizeof(rgb), 1, l_file);

		l_texture[j + 0] = rgb.rgbtRed;
		l_texture[j + 1] = rgb.rgbtGreen;
		l_texture[j + 2] = rgb.rgbtBlue;
		l_texture[j + 3] = 255;
		j += 4;
	}

	for(int k = 0;k<(int)sizeof(l_texture);++k){
        cout<<"l_texture["<<k<<"]: "<<(int)l_texture[k]<<"\n";
	}

	fclose(l_file);

	glBindTexture(GL_TEXTURE_2D, num_texture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

	// glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, infoheader.biWidth, infoheader.biHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, l_texture);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, infoheader.biWidth, infoheader.biHeight, GL_RGBA, GL_UNSIGNED_BYTE, l_texture);

	free(l_texture);

	return (num_texture);
}


void loadAllImages()
{
    printf("loading....\n");
	wallImage = loadBitmapImage("images/wall.bmp");
	skyImage = loadBitmapImage("images/grass.bmp");
	cout<<"loaded....\n";
}


void keyboardListener(unsigned char key, int x,int y){
    if(key=='z' || key=='Z'){
        R-=10;
    }else if(key=='x' || key=='X'){
        R+=10;
    }else if(key == 'w'){
        up_down += 5;
    }else if(key == 's'){
        up_down -= 5;
    }else if(key == 'd'){
        left_right += 5;
    }else if(key == 'a'){
        left_right -= 5;
    }else if(key == 'q'){
        rotate_x += 5;
    }else if(key == 'e'){
        rotate_x -= 5;
    }else if(key == 'r'){
        rotate_y -= 5;
    }else if(key == 'f'){
        rotate_y += 5;
    }

}


void specialKeyListener(int key, int x, int y){
	if (key == GLUT_KEY_UP)
	{
		vAngle+=angleSpeed;
	}
	else if (key == GLUT_KEY_DOWN)
	{
		vAngle-=angleSpeed;
	}
	else if (key == GLUT_KEY_LEFT)
	{
		hAngle-=angleSpeed;
	}
	else if (key == GLUT_KEY_RIGHT)
	{
		hAngle+=angleSpeed;
	}
	else if (key == GLUT_KEY_PAGE_UP)
	{
	    if(R>=Rspeed){
            R-=Rspeed;
	    }
	}
	else if (key == GLUT_KEY_PAGE_DOWN)
    {
        R+=Rspeed;
    }
}


void mouseListener(int button, int state, int x, int y){ 	//x, y is the x-y of the screen (2D)
    switch(button)
    {
    case GLUT_LEFT_BUTTON:
        if(state == GLUT_DOWN) 		// 2 times?? in ONE click? -- solution is checking DOWN or UP
        {
            drawaxes=1-drawaxes;
        }
        break;

    case GLUT_RIGHT_BUTTON:
        //........
        break;

    case GLUT_MIDDLE_BUTTON:
        //........
        break;

    default:
        break;
    }
}


void mouseMotion(int x, int y){
    if (x < prev_mouse_x){
        eyeX++;
        centerX++;
        R+=10;
    }
    if (x > prev_mouse_x){
        eyeX--;
        centerX--;
        R-=10;
    }
    prev_mouse_x = x;


    printf("x: %d------------y: %d\n\n", x,y);
}


void draw_minaret(){

    //----------------------------------------base of the minaret
    glBegin(GL_QUADS);{
        glColor3f(0.0f, 0.0f, 0.0f);//Black
        glVertex3d(-7, -7, 0);
        glVertex3d(7, -7, 0);
        glVertex3d(7, 7, 0);
        glVertex3d(-7, 7, 0);
    }
    glEnd();


    //----------------------------------------lower part of minaret
	glColor3f(0.5f, 0.5f, 0.5f);//Gray
    glBegin(GL_QUADS);{
        glVertex3d(7, -7, 0);
        glVertex3d(7, 7, 0);
        glVertex3d(5, 5, 15);
        glVertex3d(5, -5, 15);
    }
    glEnd();
    glBegin(GL_QUADS);{
        glVertex3d(-7, -7, 0);
        glVertex3d(7, -7, 0);
        glVertex3d(5, -5, 15);
        glVertex3d(-5, -5, 15);
    }
    glEnd();
    glBegin(GL_QUADS);{
        glVertex3d(-7, -7, 0);
        glVertex3d(-7, 7, 0);
        glVertex3d(-5, 5, 15);
        glVertex3d(-5, -5, 15);
    }
    glEnd();
    glBegin(GL_QUADS);{
        glVertex3d(-7, 7, 0);
        glVertex3d(7, 7, 0);
        glVertex3d(5, 5, 15);
        glVertex3d(-5, 5, 15);
    }
    glEnd();
    glBegin(GL_QUADS);{
        glColor3f(0.0f, 0.0f, 0.0f);//Black
        glVertex3d(-5, -5, 15);
        glVertex3d(5, -5, 15);
        glVertex3d(5, 5, 15);
        glVertex3d(-5, 5, 15);
    }
    glEnd();


    //----------------------------------------middle part of the minaret
    glColor3f(0.5f, 0.5f, 0.5f);//Gray
    glBegin(GL_QUADS);{
        glVertex3d(5, -5, 15);
        glVertex3d(5, 5, 15);
        glVertex3d(5, 5, 70);
        glVertex3d(5, -5, 70);
    }
    glEnd();
    glBegin(GL_QUADS);{
        glVertex3d(-5, -5, 15);
        glVertex3d(5, -5, 15);
        glVertex3d(5, -5, 70);
        glVertex3d(-5, -5, 70);
    }
    glEnd();
    glBegin(GL_QUADS);{
        glVertex3d(-5, -5, 15);
        glVertex3d(-5, 5, 15);
        glVertex3d(-5, 5, 70);
        glVertex3d(-5, -5, 70);
    }
    glEnd();
    glBegin(GL_QUADS);{
        glVertex3d(-5, 5, 15);
        glVertex3d(5, 5, 15);
        glVertex3d(5, 5, 70);
        glVertex3d(-5, 5, 70);
    }
    glEnd();
    glBegin(GL_QUADS);{
        glColor3f(0.0f, 0.0f, 0.0f);//Black
        glVertex3d(-5, -5, 70);
        glVertex3d(5, -5, 70);
        glVertex3d(5, 5, 70);
        glVertex3d(-5, 5, 70);
    }
    glEnd();

    //----------------------------------------middle-top part of the minaret
    glColor3f(0.5f, 0.5f, 0.5f);//Gray
    glBegin(GL_QUADS);{
        glVertex3d(5, -5, 70);
        glVertex3d(5, -3, 70);
        glVertex3d(5, -3, 75);
        glVertex3d(5, -5, 75);
    }
    glEnd();
    glBegin(GL_QUADS);{
        glVertex3d(3, -5, 70);
        glVertex3d(3, -3, 70);
        glVertex3d(3, -3, 75);
        glVertex3d(3, -5, 75);
    }
    glEnd();
    glBegin(GL_QUADS);{
        glVertex3d(3, -5, 70);
        glVertex3d(5, -5, 70);
        glVertex3d(5, -3, 75);
        glVertex3d(3, -3, 75);
    }
    glEnd();
    glBegin(GL_QUADS);{
        glVertex3d(3, -3, 70);
        glVertex3d(5, -3, 70);
        glVertex3d(5, -3, 75);
        glVertex3d(3, -3, 75);
    }
    glEnd();
    glBegin(GL_QUADS);{
        glColor3f(0.0f, 0.0f, 0.0f);//Black
        glVertex3d(5, -5, 75);
        glVertex3d(5, -3, 75);
        glVertex3d(3, -3, 75);
        glVertex3d(3, -5, 75);
    }
    glEnd();
    glPushMatrix();{
        glTranslated(0, 8, 0);
        glColor3f(0.5f, 0.5f, 0.5f);//Gray
        glBegin(GL_QUADS);{
            glVertex3d(5, -5, 70);
            glVertex3d(5, -3, 70);
            glVertex3d(5, -3, 75);
            glVertex3d(5, -5, 75);
        }
        glEnd();
        glBegin(GL_QUADS);{
            glVertex3d(3, -5, 70);
            glVertex3d(3, -3, 70);
            glVertex3d(3, -3, 75);
            glVertex3d(3, -5, 75);
        }
        glEnd();
        glBegin(GL_QUADS);{
            glVertex3d(3, -5, 70);
            glVertex3d(5, -5, 70);
            glVertex3d(5, -3, 75);
            glVertex3d(3, -3, 75);
        }
        glEnd();
        glBegin(GL_QUADS);{
            glVertex3d(3, -3, 70);
            glVertex3d(5, -3, 70);
            glVertex3d(5, -3, 75);
            glVertex3d(3, -3, 75);
        }
        glEnd();
        glBegin(GL_QUADS);{
            glColor3f(0.0f, 0.0f, 0.0f);//Black
            glVertex3d(5, -5, 75);
            glVertex3d(5, -3, 75);
            glVertex3d(3, -3, 75);
            glVertex3d(3, -5, 75);
        }
        glEnd();
    }
    glPopMatrix();
    glPushMatrix();{
        glTranslated(-8, 0, 0);
        glColor3f(0.5f, 0.5f, 0.5f);//Gray
        glBegin(GL_QUADS);{
            glVertex3d(5, -5, 70);
            glVertex3d(5, -3, 70);
            glVertex3d(5, -3, 75);
            glVertex3d(5, -5, 75);
        }
        glEnd();
        glBegin(GL_QUADS);{
            glVertex3d(3, -5, 70);
            glVertex3d(3, -3, 70);
            glVertex3d(3, -3, 75);
            glVertex3d(3, -5, 75);
        }
        glEnd();
        glBegin(GL_QUADS);{
            glVertex3d(3, -5, 70);
            glVertex3d(5, -5, 70);
            glVertex3d(5, -3, 75);
            glVertex3d(3, -3, 75);
        }
        glEnd();
        glBegin(GL_QUADS);{
            glVertex3d(3, -3, 70);
            glVertex3d(5, -3, 70);
            glVertex3d(5, -3, 75);
            glVertex3d(3, -3, 75);
        }
        glEnd();
        glBegin(GL_QUADS);{
            glColor3f(0.0f, 0.0f, 0.0f);//Black
            glVertex3d(5, -5, 75);
            glVertex3d(5, -3, 75);
            glVertex3d(3, -3, 75);
            glVertex3d(3, -5, 75);
        }
        glEnd();
    }
    glPopMatrix();
    glPushMatrix();{
        glTranslated(-8, 8, 0);
        glColor3f(0.5f, 0.5f, 0.5f);//Gray
        glBegin(GL_QUADS);{
            glVertex3d(5, -5, 70);
            glVertex3d(5, -3, 70);
            glVertex3d(5, -3, 75);
            glVertex3d(5, -5, 75);
        }
        glEnd();
        glBegin(GL_QUADS);{
            glVertex3d(3, -5, 70);
            glVertex3d(3, -3, 70);
            glVertex3d(3, -3, 75);
            glVertex3d(3, -5, 75);
        }
        glEnd();
        glBegin(GL_QUADS);{
            glVertex3d(3, -5, 70);
            glVertex3d(5, -5, 70);
            glVertex3d(5, -3, 75);
            glVertex3d(3, -3, 75);
        }
        glEnd();
        glBegin(GL_QUADS);{
            glVertex3d(3, -3, 70);
            glVertex3d(5, -3, 70);
            glVertex3d(5, -3, 75);
            glVertex3d(3, -3, 75);
        }
        glEnd();
        glBegin(GL_QUADS);{
            glColor3f(0.0f, 0.0f, 0.0f);//Black
            glVertex3d(5, -5, 75);
            glVertex3d(5, -3, 75);
            glVertex3d(3, -3, 75);
            glVertex3d(3, -5, 75);
        }
        glEnd();
    }
    glPopMatrix();
    glBegin(GL_QUADS);{
        glColor3f(0.0f, 0.0f, 0.0f);//Black
        glVertex3d(-5, -5, 75);
        glVertex3d(5, -5, 75);
        glVertex3d(5, 5, 75);
        glVertex3d(-5, 5, 75);
    }
    glEnd();


    //----------------------------------------top part of the minaret
    glColor3f(0.5f, 0.5f, 0.5f);//Gray
    glBegin(GL_TRIANGLES);{
        glVertex3d(5, -5, 75);
        glVertex3d(5, 5, 75);
        glVertex3d(0, 0, 105);
    }
    glEnd();
    glBegin(GL_TRIANGLES);{
        glVertex3d(5, 5, 75);
        glVertex3d(-5, 5, 75);
        glVertex3d(0, 0, 105);
    }
    glEnd();
    glBegin(GL_TRIANGLES);{
        glVertex3d(-5, 5, 75);
        glVertex3d(-5, -5, 75);
        glVertex3d(0, 0, 105);
    }
    glEnd();
    glBegin(GL_TRIANGLES);{
        glVertex3d(-5, -5, 75);
        glVertex3d(5, -5, 75);
        glVertex3d(0, 0, 105);
    }
    glEnd();

}


void four_minarets(){
    glPushMatrix();{
        glTranslated(-100, -100, 0);
        draw_minaret();
    }
    glPopMatrix();
    glPushMatrix();{
        glTranslated(-100, 100, 0);
        draw_minaret();
    }
    glPopMatrix();
    glPushMatrix();{
        glTranslated(100, 100, 0);
        draw_minaret();
    }
    glPopMatrix();
    glPushMatrix();{
        glTranslated(100, -100, 0);
        draw_minaret();
    }
    glPopMatrix();
}


void draw_sanctuary(){
    //side triangle
    glBegin(GL_TRIANGLES);{
        glColor3f(0.5f, 0.5f, 0.5f); //Black
        glVertex3d(-88, -2, 0);
        glVertex3d(88, -2, 0);
        glVertex3d(0, -2, 38);
    }
    glEnd();
    //**************************************

    //outlines
    glBegin(GL_LINES);{
        glColor3f(0,0,0);
        glVertex3d(0, -90, 40);
        glVertex3d(0, 0, 40);
    }
    glEnd();
    glBegin(GL_LINES);{
        glColor3f(0,0,0);
        glVertex3d(0, -90, 40);
        glVertex3d(-90, 0, 0);
    }
    glEnd();
    glBegin(GL_LINES);{
        glColor3f(0,0,0);
        glVertex3d(0, 0, 40);
        glVertex3d(0, 0, 38);
    }
    glEnd();
    //**************************************

    //left triangle
    glBegin(GL_TRIANGLES);{
        glColor3f(0.9f, 0.9f, 0.9f); //light-Gray
        glVertex3d(0, -90, 40);
        glVertex3d(-90, 0, 0);
        glVertex3d(0, 0, 40);
    }
    glEnd();

    glBegin(GL_TRIANGLES);{
        glColor3f(0.7f, 0.7f, 0.7f); //slight-darker-Gray
        glVertex3d(0, -88, 38);
        glVertex3d(-88, 0, 0);
        glVertex3d(0, 0, 38);
    }
    glEnd();
    //**************************************

    //right triangle
    glBegin(GL_TRIANGLES);{
        glColor3f(0.9f, 0.9f, 0.9f); //light-Gray
        glVertex3d(0, -90, 40);
        glVertex3d(90, 0, 0);
        glVertex3d(0, 0, 40);
    }
    glEnd();
    glBegin(GL_TRIANGLES);{
        glColor3f(0.7f, 0.7f, 0.7f); //slight-darker-Gray
        glVertex3d(0, -88, 38);
        glVertex3d(88, 0, 0);
        glVertex3d(0, 0, 38);
    }
    glEnd();
    //**************************************

    //front quads
    //right quad
    glBegin(GL_QUADS);{
        glColor3f(0.5f, 0.5f, 0.5f);//darker gray
        glVertex3d(-90, 0, 0);
        glVertex3d(-88, 0, 0);
        glVertex3d(0, 0, 38);
        glVertex3d(0, 0, 40);
    }
    glEnd();
    //left quad
    glBegin(GL_QUADS);{
        glColor3f(0.5f, 0.5f, 0.5f);//darker gray
        glVertex3d(90, 0, 0);
        glVertex3d(88, 0, 0);
        glVertex3d(0, 0, 38);
        glVertex3d(0, 0, 40);
    }
    glEnd();
}


void toppest_part(){
    glBegin(GL_TRIANGLES);{
        glColor3f(0.1f, 0.1f, 0.0f);//Bronze
        glVertex3d(5, 0, 40);
        glVertex3d(0, 0, 40);
        glVertex3d(0, 0, 75);
    }
    glEnd();
    glBegin(GL_TRIANGLES);{
        glColor4f(1.0f, 1.0f, 0.0f, 0.0f);//yellow
        glVertex3d(5, 0, 40);
        glVertex3d(0, 0, 75);
        glVertex3d(5, 5, 40);
    }
    glEnd();
    glBegin(GL_TRIANGLES);{
        glColor4f(1.0f, 1.0f, 0.0f, 0.0f);//yellow
        glVertex3d(5, 0, 40);
        glVertex3d(0, 0, 75);
        glVertex3d(5, -5, 40);
    }
    glEnd();
}


void top_structure(){
    glBegin(GL_QUADS);{
        glColor3f(0.5f, 0.5f, 0.5f); //light-Gray
        glVertex3d(0, 0, 35);
        glVertex3d(0, 0, 50);
        glVertex3d(15, 0, 65);
        glVertex3d(15, 0, 35);
    }
    glEnd();
    glBegin(GL_QUADS);{
        glColor3f(0.5f, 0.5f, 0.5f); //light-Gray
        glVertex3d(15, 0, 65);
        glVertex3d(15, 0, 35);
        glVertex3d(7, 7, 35);
        glVertex3d(7, 7, 45);
    }
    glEnd();
    glBegin(GL_QUADS);{
        glColor3f(0.5f, 0.5f, 0.5f); //light-Gray
        glVertex3d(15, 0, 65);
        glVertex3d(15, 0, 35);
        glVertex3d(7, -7, 35);
        glVertex3d(7, -7, 45);
    }
    glEnd();
    glBegin(GL_TRIANGLES);{
        glColor3f(0.5f, 0.5f, 0.5f); //light-Gray //black
        glVertex3d(15, 0, 65);
        glVertex3d(0, 0, 50);
        glVertex3d(7, -7, 45);
    }
    glEnd();
    glBegin(GL_TRIANGLES);{
        glColor3f(0.5f, 0.5f, 0.5f); //light-Gray //black
        glVertex3d(15, 0, 65);
        glVertex3d(0, 0, 50);
        glVertex3d(7, 7, 45);
    }
    glEnd();
    glBegin(GL_LINES);{
        glColor3f(1.0f, 1.0f, 1.0f); //white
        glVertex3d(15, 0, 65);
        glVertex3d(0, 0, 50);
    }
    glEnd();
    glBegin(GL_LINES);{
        glColor3f(0.0f, 0.0f, 0.0f);//black
        glVertex3d(15, 0, 65);
        glVertex3d(15, 0, 35);
    }
    glEnd();
    glBegin(GL_LINES);{
        glColor3f(0.0f, 0.0f, 0.0f); //black
        glVertex3d(7, -7, 45);
        glVertex3d(7, -7, 35);
    }
    glEnd();
    toppest_part();
}


void full_top_structure(){
    glPushMatrix();{
        glRotated(45, 0, 0, 1);
        top_structure();
        glPushMatrix();{
            glRotated(90, 0, 0, 1);
            top_structure();
        }
        glPopMatrix();
        glPushMatrix();{
            glRotated(180, 0, 0, 1);
            top_structure();
        }
        glPopMatrix();
        glPushMatrix();{
            glRotated(270, 0, 0, 1);
            top_structure();
        }
        glPopMatrix();
    }glPopMatrix();
}


void full_sanctuary(){

    glPushMatrix();{
        glTranslated(0, 90, 0);
        draw_sanctuary();
    }
    glPopMatrix();
    glPushMatrix();{
        glTranslated(0, -90, 0);
        glRotated(180, 0,0,1);
        draw_sanctuary();
    }
    glPopMatrix();
    glPushMatrix();{
        glTranslated(-90, 0, 0);
        glRotated(90, 0,0,1);
        draw_sanctuary();
    }
    glPopMatrix();
    glPushMatrix();{
        glTranslated(90, 0, 0);
        glRotated(-90, 0,0,1);
        draw_sanctuary();
    }
    glPopMatrix();
    glBegin(GL_QUADS);{
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex3d(-89, 40, 0);
        glVertex3d(-89, -40, 0);
        glVertex3d(-89, -40, 10);
        glVertex3d(-89, 40, 10);
    }
    glEnd();
}


void entrance_shade(){
    glPushMatrix();{
        glTranslated(-5.0, 0, 0);
        glBegin(GL_QUADS);{
            glColor3f(0.0f, 0.0f, 0.0f);
            glVertex3d(-1.5, -1.5, 0);
            glVertex3d(1.5, -1.5, 0);
            glVertex3d(1.5, 1.5, 0);
            glVertex3d(-1.5, 1.5, 0);
        }
        glEnd();
        glBegin(GL_QUADS);{
            glColor3f(0.8f, 0.8f, 0.8f);
            glVertex3d(-1.5, -1.5, 0);
            glVertex3d(1.5, -1.5, 0);
            glVertex3d(1.5, -1.5, 12);
            glVertex3d(-1.5, -1.5, 12);
        }
        glEnd();
        glBegin(GL_QUADS);{
            glColor3f(0.8f, 0.8f, 0.8f);
            glVertex3d(1.5, -1.5, 0);
            glVertex3d(1.5, 1.5, 0);
            glVertex3d(1.5, 1.5, 12);
            glVertex3d(1.5, -1.5, 12);
        }
        glEnd();
        glBegin(GL_QUADS);{
            glColor3f(0.8f, 0.8f, 0.8f);
            glVertex3d(1.5, 1.5, 0);
            glVertex3d(-1.5, 1.5, 0);
            glVertex3d(-1.5, 1.5, 12);
            glVertex3d(1.5, 1.5, 12);
        }
        glEnd();
        glBegin(GL_QUADS);{
            glColor3f(0.8f, 0.8f, 0.8f);
            glVertex3d(-1.5, 1.5, 0);
            glVertex3d(-1.5, -1.5, 0);
            glVertex3d(-1.5, -1.5, 12);
            glVertex3d(-1.5, 1.5, 12);
        }
        glEnd();
        glBegin(GL_QUADS);{
            glColor3f(0.0f, 0.0f, 0.0f);
            glVertex3d(-1.5, -1.5, 12);
            glVertex3d(1.5, -1.5, 12);
            glVertex3d(1.5, 1.5, 12);
            glVertex3d(-1.5, 1.5, 12);
        }
        glEnd();
    }glPopMatrix();


    glPushMatrix();{
        glTranslated(5.0, 0, 0);
        glBegin(GL_QUADS);{
            glColor3f(0.0f, 0.0f, 0.0f);
            glVertex3d(-1.5, -1.5, 0);
            glVertex3d(1.5, -1.5, 0);
            glVertex3d(1.5, 1.5, 0);
            glVertex3d(-1.5, 1.5, 0);
        }
        glEnd();
        glBegin(GL_QUADS);{
            glColor3f(0.8f, 0.8f, 0.8f);
            glVertex3d(-1.5, -1.5, 0);
            glVertex3d(1.5, -1.5, 0);
            glVertex3d(1.5, -1.5, 12);
            glVertex3d(-1.5, -1.5, 12);
        }
        glEnd();
        glBegin(GL_QUADS);{
            glColor3f(0.8f, 0.8f, 0.8f);
            glVertex3d(1.5, -1.5, 0);
            glVertex3d(1.5, 1.5, 0);
            glVertex3d(1.5, 1.5, 12);
            glVertex3d(1.5, -1.5, 12);
        }
        glEnd();
        glBegin(GL_QUADS);{
            glColor3f(0.8f, 0.8f, 0.8f);
            glVertex3d(1.5, 1.5, 0);
            glVertex3d(-1.5, 1.5, 0);
            glVertex3d(-1.5, 1.5, 12);
            glVertex3d(1.5, 1.5, 12);
        }
        glEnd();
        glBegin(GL_QUADS);{
            glColor3f(0.8f, 0.8f, 0.8f);
            glVertex3d(-1.5, 1.5, 0);
            glVertex3d(-1.5, -1.5, 0);
            glVertex3d(-1.5, -1.5, 12);
            glVertex3d(-1.5, 1.5, 12);
        }
        glEnd();
        glBegin(GL_QUADS);{
            glColor3f(0.0f, 0.0f, 0.0f);
            glVertex3d(-1.5, -1.5, 12);
            glVertex3d(1.5, -1.5, 12);
            glVertex3d(1.5, 1.5, 12);
            glVertex3d(-1.5, 1.5, 12);
        }
        glEnd();
    }glPopMatrix();

    glBegin(GL_QUADS);{
        glColor3f(0.9f, 0.9f, 0.9f);
        glVertex3d(-6.5, -1.5, 12);
        glVertex3d(6.5, -1.5, 12);
        glVertex3d(6.5, 1.5, 12);
        glVertex3d(-6.5, 1.5, 12);
    }
    glEnd();
    glBegin(GL_QUADS);{
        glColor3f(0.9f, 0.9f, 0.9f);
        glVertex3d(-10, -5, 15.5);
        glVertex3d(10, -5, 15.5);
        glVertex3d(10, 5, 15.5);
        glVertex3d(-10, 5, 15.5);
    }
    glEnd();
    glBegin(GL_QUADS);{
        glColor3f(0.9f, 0.9f, 0.9f);
        glVertex3d(-10, -5, 16);
        glVertex3d(10, -5, 16);
        glVertex3d(10, 5, 16);
        glVertex3d(-10, 5, 16);
    }
    glEnd();
    glBegin(GL_QUADS);{
        glColor3f(0.9f, 0.9f, 0.9f);
        glVertex3d(-6.5, -1.5, 12);
        glVertex3d(6.5, -1.5, 12);
        glVertex3d(10, -5, 15.5);
        glVertex3d(-10, -5, 15.5);
    }
    glEnd();
    glBegin(GL_QUADS);{
        glColor3f(0.9f, 0.9f, 0.9f);
        glVertex3d(6.5, -1.5, 12);
        glVertex3d(6.5, 1.5, 12);
        glVertex3d(10, 5, 15.5);
        glVertex3d(10, -5, 15.5);
    }
    glEnd();
    glBegin(GL_QUADS);{
        glColor3f(0.9f, 0.9f, 0.9f);
        glVertex3d(6.5, 1.5, 12);
        glVertex3d(-6.5, 1.5, 12);
        glVertex3d(-10, 5, 15.5);
        glVertex3d(10, 5, 15.5);
    }
    glEnd();
    glBegin(GL_QUADS);{
        glColor3f(0.9f, 0.9f, 0.9f);
        glVertex3d(-6.5, 1.5, 12);
        glVertex3d(-6.5, -1.5, 12);
        glVertex3d(-10, -5, 15.5);
        glVertex3d(-10, 5, 15.5);
    }
    glEnd();
    glBegin(GL_QUADS);{
        glColor3f(0.8f, 0.8f, 0.8f);
        glVertex3d(-10, -5, 15.5);
        glVertex3d(-10, -5, 16);
        glVertex3d(-10, 5, 16);
        glVertex3d(-10, 5, 15.5);
    }
    glEnd();
    glBegin(GL_QUADS);{
        glColor3f(0.8f, 0.8f, 0.8f);
        glVertex3d(-10, -5, 15.5);
        glVertex3d(-10, -5, 16);
        glVertex3d(10, -5, 16);
        glVertex3d(10, -5, 15.5);
    }
    glEnd();
    glBegin(GL_QUADS);{
        glColor3f(0.8f, 0.8f, 0.8f);
        glVertex3d(10, -5, 15.5);
        glVertex3d(10, -5, 16);
        glVertex3d(10, 5, 16);
        glVertex3d(10, 5, 15.5);
    }
    glEnd();
    glBegin(GL_QUADS);{
        glColor3f(0.8f, 0.8f, 0.8f);
        glVertex3d(10, 5, 15.5);
        glVertex3d(10, 5, 16);
        glVertex3d(-10, 5, 16);
        glVertex3d(-10, 5, 15.5);
    }
    glEnd();
}


void entrance_full_shade(){
    glPushMatrix();{
        glTranslated(-98.5, 0, 0);
        entrance_shade();
    }
    glPopMatrix();
    glPushMatrix();{
        glTranslated(-98.5, 10, 0);
        entrance_shade();
    }
    glPopMatrix();
    glPushMatrix();{
        glTranslated(-98.5, -10, 0);
        entrance_shade();
    }
    glPopMatrix();
    glPushMatrix();{
        glTranslated(-98.5, 20, 0);
        entrance_shade();
    }
    glPopMatrix();
    glPushMatrix();{
        glTranslated(-98.5, -20, 0);
        entrance_shade();
    }
    glPopMatrix();
    glPopMatrix();
    glPushMatrix();{
        glTranslated(-98.5, 30, 0);
        entrance_shade();
    }
    glPopMatrix();
    glPopMatrix();
    glPushMatrix();{
        glTranslated(-98.5, -30, 0);
        entrance_shade();
    }
    glPopMatrix();
    glPushMatrix();{
        glTranslated(-98.5, 40, 0);
        entrance_shade();
    }
    glPopMatrix();
    glPopMatrix();
    glPushMatrix();{
        glTranslated(-98.5, -40, 0);
        entrance_shade();
    }
    glPopMatrix();
}


void side_shade(){
    glPushMatrix();{
        //*****************************Pillar************************
        glBegin(GL_QUADS);{
            glColor3f(0.0f, 0.0f, 0.0f);
            glVertex3d(-1.5, -1.5, 0);
            glVertex3d(1.5, -1.5, 0);
            glVertex3d(1.5, 1.5, 0);
            glVertex3d(-1.5, 1.5, 0);
        }
        glEnd();
        glBegin(GL_QUADS);{
            glColor3f(0.8f, 0.8f, 0.8f);
            glVertex3d(-1.5, -1.5, 0);
            glVertex3d(1.5, -1.5, 0);
            glVertex3d(1.5, -1.5, 6);
            glVertex3d(-1.5, -1.5, 6);
        }
        glEnd();
        glBegin(GL_QUADS);{
            glColor3f(0.8f, 0.8f, 0.8f);
            glVertex3d(1.5, -1.5, 0);
            glVertex3d(1.5, 1.5, 0);
            glVertex3d(1.5, 1.5, 6);
            glVertex3d(1.5, -1.5, 6);
        }
        glEnd();
        glBegin(GL_QUADS);{
            glColor3f(0.8f, 0.8f, 0.8f);
            glVertex3d(1.5, 1.5, 0);
            glVertex3d(-1.5, 1.5, 0);
            glVertex3d(-1.5, 1.5, 6);
            glVertex3d(1.5, 1.5, 6);
        }
        glEnd();
        glBegin(GL_QUADS);{
            glColor3f(0.8f, 0.8f, 0.8f);
            glVertex3d(-1.5, 1.5, 0);
            glVertex3d(-1.5, -1.5, 0);
            glVertex3d(-1.5, -1.5, 6);
            glVertex3d(-1.5, 1.5, 6);
        }
        glEnd();
        glBegin(GL_QUADS);{
            glColor3f(0.0f, 0.0f, 0.0f);
            glVertex3d(-1.5, -1.5, 6);
            glVertex3d(1.5, -1.5, 6);
            glVertex3d(1.5, 1.5, 6);
            glVertex3d(-1.5, 1.5, 6);
        }
        glEnd();

        //********************************upper quad****************************
        glBegin(GL_QUADS);{
            glColor3f(0.7f, 0.7f, 0.7f);
            glVertex3d(-6, -6, 8);
            glVertex3d(6, -6, 8);
            glVertex3d(6, 6, 8);
            glVertex3d(-6, 6, 8);
        }
        glEnd();
        //********************************lower quad****************************
        glBegin(GL_QUADS);{
            glColor3f(0.4f, 0.4f, 0.4f);
            glVertex3d(-6, -6, 9);
            glVertex3d(6, -6, 9);
            glVertex3d(6, 6, 9);
            glVertex3d(-6, 6, 9);
        }
        glEnd();
        //******************************side quads******************************
        glBegin(GL_QUADS);{
            glColor3f(0.6f, 0.6f, 0.6f);
            glVertex3d(-1.5, -1.5, 6);
            glVertex3d(-6, -6, 8);
            glVertex3d(6, -6, 8);
            glVertex3d(1.5, -1.5, 6);
        }
        glEnd();
        glBegin(GL_QUADS);{
            glColor3f(0.6f, 0.6f, 0.6f);
            glVertex3d(1.5, -1.5, 6);
            glVertex3d(6, -6, 8);
            glVertex3d(6, 6, 8);
            glVertex3d(1.5, 1.5, 6);
        }
        glEnd();
        glBegin(GL_QUADS);{
            glColor3f(0.6f, 0.6f, 0.6f);
            glVertex3d(1.5, 1.5, 6);
            glVertex3d(6, 6, 8);
            glVertex3d(-6, 6, 8);
            glVertex3d(-1.5, 1.5, 6);
        }
        glEnd();
        glBegin(GL_QUADS);{
            glColor3f(0.6f, 0.6f, 0.6f);
            glVertex3d(-1.5, 1.5, 6);
            glVertex3d(-6, 6, 8);
            glVertex3d(-6, -6, 8);
            glVertex3d(-1.5, -1.5, 6);
        }
        glEnd();
        //******************************side borders*******************************
        glBegin(GL_QUADS);{
            glColor3f(0.6f, 0.6f, 0.6f);
            glVertex3d(-6, -6, 8);
            glVertex3d(-6, -6, 9);
            glVertex3d(-6, 6, 9);
            glVertex3d(-6, 6, 8);
        }
        glEnd();
        glBegin(GL_QUADS);{
            glColor3f(0.6f, 0.6f, 0.6f);
            glVertex3d(-6, 6, 9);
            glVertex3d(-6, 6, 8);
            glVertex3d(6, 6, 8);
            glVertex3d(6, 6, 9);
        }
        glEnd();
        glBegin(GL_QUADS);{
            glColor3f(0.6f, 0.6f, 0.6f);
            glVertex3d(6, 6, 9);
            glVertex3d(6, 6, 8);
            glVertex3d(6, -6, 8);
            glVertex3d(6, -6, 9);
        }
        glEnd();
        glBegin(GL_QUADS);{
            glColor3f(0.6f, 0.6f, 0.6f);
            glVertex3d(6, -6, 9);
            glVertex3d(6, -6, 8);
            glVertex3d(-6, -6, 8);
            glVertex3d(-6, -6, 9);
        }
        glEnd();
    }glPopMatrix();
}


void full_side_shades(){
    glPushMatrix();{
        glTranslated(-102, 48, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-102, -48, 0);
        side_shade();
    }glPopMatrix();

    glPushMatrix();{
        glTranslated(-114, 48, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-114, -48, 0);
        side_shade();
    }glPopMatrix();

    glPushMatrix();{
        glTranslated(-114, 60, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-114, -60, 0);
        side_shade();
    }glPopMatrix();

    glPushMatrix();{
        glTranslated(-114, 72, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-114, -72, 0);
        side_shade();
    }glPopMatrix();

    glPushMatrix();{
        glTranslated(-114, 84, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-114, -84, 0);
        side_shade();
    }glPopMatrix();

    glPushMatrix();{
        glTranslated(-114, 96, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-114, -96, 0);
        side_shade();
    }glPopMatrix();

    glPushMatrix();{
        glTranslated(-114, 108, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-114, -108, 0);
        side_shade();
    }glPopMatrix();

    //*********************************right side shades
    glPushMatrix();{
        glTranslated(-114, -120, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-114, -132, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-126, -132, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-138, -132, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-150, -132, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-162, -132, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-174, -132, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-186, -132, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-198, -132, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-210, -132, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-222, -132, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-234, -132, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-246, -132, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-258, -132, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-270, -132, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-282, -132, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-294, -132, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-306, -132, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-318, -132, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-330, -132, 0);
        side_shade();
    }glPopMatrix();


    glPushMatrix();{
        glTranslated(-114, -120, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-126, -120, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-138, -120, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-150, -120, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-162, -120, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-174, -120, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-186, -120, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-198, -120, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-210, -120, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-222, -120, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-234, -120, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-246, -120, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-258, -120, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-270, -120, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-282, -120, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-294, -120, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-306, -120, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-318, -120, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-330, -120, 0);
        side_shade();
    }glPopMatrix();



    //*****************************middle side shades
    glPushMatrix();{
        glTranslated(-330, -108, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-330, -96, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-330, -84, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-330, -72, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-330, -60, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-330, -48, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-330, -36, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-330, -24, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-330, -12, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-330, 0, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-330, 12, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-330, 24, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-330, 36, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-330, 48, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-330, 60, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-330, 72, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-330, 84, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-330, 96, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-330, 108, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-330, 120, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-330, 132, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-330, 144, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-330, 156, 0);
        side_shade();
    }glPopMatrix();

    glPushMatrix();{
        glTranslated(-342, -108, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-342, -96, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-342, -84, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-342, -72, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-342, -60, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-342, -48, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-342, -36, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-342, -24, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-342, 24, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-342, 36, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-342, 48, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-342, 60, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-342, 72, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-342, 84, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-342, 96, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-342, 108, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-342, 120, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-342, 132, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-342, 144, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-342, 156, 0);
        side_shade();
    }glPopMatrix();

    //***********************************left side shades
    glPushMatrix();{
        glTranslated(-110, 120, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-110, 132, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-110, 144, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-110, 156, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-122, 120, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-122, 132, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-122, 144, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-122, 156, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-134, 120, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-134, 132, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-134, 144, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-134, 156, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-146, 132, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-146, 144, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-146, 156, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-318, 156, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-306, 156, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-294, 156, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-282, 156, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-270, 156, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-318, 144, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-306, 144, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-294, 144, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-282, 144, 0);
        side_shade();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-270, 144, 0);
        side_shade();
    }glPopMatrix();

}


void staires(){
    glBegin(GL_QUADS);{
        glColor3f(0.9, 0.9, 0.90);
        glVertex3d(-255, 144, 0);
        glVertex3d(-255, 168, 0);
        glVertex3d(-215, 168, -20);
        glVertex3d(-215, 144, -20);
    }
    glEnd();
    glBegin(GL_TRIANGLES);{
        glColor3f(0.85, 0.85, 0.85);
        glVertex3d(-255, 144, 0);
        glVertex3d(-255, 144, -20);
        glVertex3d(-215, 144, -20);
    }
    glEnd();
    glBegin(GL_TRIANGLES);{
        glColor3f(0.85, 0.85, 0.85);
        glVertex3d(-255, 144, 0);
        glVertex3d(-255, 144, -20);
        glVertex3d(-215, 144, -20);
    }
    glEnd();
    glBegin(GL_TRIANGLES);{
        glColor3f(0.85, 0.85, 0.85);
        glVertex3d(-255, 168, 0);
        glVertex3d(-255, 168, -20);
        glVertex3d(-215, 168, -20);
    }
    glEnd();
    glBegin(GL_TRIANGLES);{
        glColor3f(0.85, 0.85, 0.85);
        glVertex3d(-255, 168, 0);
        glVertex3d(-255, 168, -20);
        glVertex3d(-215, 168, -20);
    }
    glEnd();
}


void full_base(){
    glBegin(GL_QUADS);{
        glColor3f(0.95, 0.95, 0.95);
        glVertex3d(130, 120, 0);
        glVertex3d(130, -120, 0);
        glVertex3d(-130, -120, 0);
        glVertex3d(-130, 120, 0);
    }
    glEnd();
    glBegin(GL_QUADS);{
        glColor3f(0.95, 0.95, 0.95);
        glVertex3d(-100, 140, 0);
        glVertex3d(-100, -140, 0);
        glVertex3d(-350, -140, 0);
        glVertex3d(-350, 140, 0);
    }
    glEnd();
    glBegin(GL_QUADS);{
        glColor3f(0.95, 0.95, 0.95);
        glVertex3d(-350, 140, 0);
        glVertex3d(-350, 168, 0);
        glVertex3d(-255, 168, 0);
        glVertex3d(-255, 140, 0);
    }
    glEnd();
    glBegin(GL_QUADS);{
        glColor3f(0.95, 0.95, 0.95);
        glVertex3d(-100, 120, 0);
        glVertex3d(-100, 170, 0);
        glVertex3d(-200, 170, 0);
        glVertex3d(-200, 120, 0);
    }
    glEnd();
}


void base_side_wall(){
    glBegin(GL_QUADS);{
        glColor3f(0.85, 0.85, 0.85);
        glVertex3d(130, 120, 0);
        glVertex3d(130, 120, -20);
        glVertex3d(130, -120, -20);
        glVertex3d(130, -120, 0);
    }
    glEnd();
    glBegin(GL_QUADS);{
        glColor3f(0.85, 0.85, 0.85);
        glVertex3d(130, -120, 0);
        glVertex3d(130, -120, -20);
        glVertex3d(-100, -120, -20);
        glVertex3d(-100, -120, 0);
    }
    glEnd();
    glBegin(GL_QUADS);{
        glColor3f(0.85, 0.85, 0.85);
        glVertex3d(-100, -120, -20);
        glVertex3d(-100, -120, 0);
        glVertex3d(-100, -140, 0);
        glVertex3d(-100, -140, -20);
    }
    glEnd();
    glBegin(GL_QUADS);{
        glColor3f(0.85, 0.85, 0.85);
        glVertex3d(-100, -140, -20);
        glVertex3d(-100, -140, 0);
        glVertex3d(-350, -140, 0);
        glVertex3d(-350, -140, -20);
    }
    glEnd();
    glBegin(GL_QUADS);{
        glColor3f(0.85, 0.85, 0.85);
        glVertex3d(-350, -140, -20);
        glVertex3d(-350, -140, 0);
        glVertex3d(-350, 168, 0);
        glVertex3d(-350, 168, -20);
    }
    glEnd();
    glBegin(GL_QUADS);{
        glColor3f(0.85, 0.85, 0.85);
        glVertex3d(-350, 168, -20);
        glVertex3d(-350, 168, 0);
        glVertex3d(-255, 168, 0);
        glVertex3d(-255, 168, -20);
    }
    glEnd();
    glBegin(GL_QUADS);{
        glColor3f(0.85, 0.85, 0.85);
        glVertex3d(-255, 168, -20);
        glVertex3d(-255, 168, 0);
        glVertex3d(-255, 140, 0);
        glVertex3d(-255, 140, -20);
    }
    glEnd();
    glBegin(GL_QUADS);{
        glColor3f(0.85, 0.85, 0.85);
        glVertex3d(-255, 140, -20);
        glVertex3d(-255, 140, 0);
        glVertex3d(-200, 140, 0);
        glVertex3d(-200, 140, -20);
    }
    glEnd();
    glBegin(GL_QUADS);{
        glColor3f(0.85, 0.85, 0.85);
        glVertex3d(-200, 140, -20);
        glVertex3d(-200, 140, 0);
        glVertex3d(-200, 170, 0);
        glVertex3d(-200, 170, -20);
    }
    glEnd();
    glBegin(GL_QUADS);{
        glColor3f(0.85, 0.85, 0.85);
        glVertex3d(-200, 170, -20);
        glVertex3d(-200, 170, 0);
        glVertex3d(-100, 170, 0);
        glVertex3d(-100, 170, -20);
    }
    glEnd();
    glBegin(GL_QUADS);{
        glColor3f(0.85, 0.85, 0.85);
        glVertex3d(-100, 170, -20);
        glVertex3d(-100, 170, 0);
        glVertex3d(-100, 120, 0);
        glVertex3d(-100, 120, -20);
    }
    glEnd();
    glBegin(GL_QUADS);{
        glColor3f(0.85, 0.85, 0.85);
        glVertex3d(-100, 120, -20);
        glVertex3d(-100, 100, 0);
        glVertex3d(130, 120, 0);
        glVertex3d(130, 120, -20);
    }
    glEnd();
}


void water_source(){
    glColor3f(0.9, 0.9, 0.9);
    glBegin(GL_QUADS);{
            glVertex3d(-1.5, -1.5, 0);
            glVertex3d(-6, -6, 3);
            glVertex3d(6, -6, 3);
            glVertex3d(1.5, -1.5, 0);
        }
        glEnd();
        glBegin(GL_QUADS);{
            glVertex3d(1.5, -1.5, 0);
            glVertex3d(6, -6, 3);
            glVertex3d(6, 6, 3);
            glVertex3d(1.5, 1.5, 0);
        }
        glEnd();
        glBegin(GL_QUADS);{
            glVertex3d(1.5, 1.5, 0);
            glVertex3d(6, 6, 3);
            glVertex3d(-6, 6, 3);
            glVertex3d(-1.5, 1.5, 0);
        }
        glEnd();
        glBegin(GL_QUADS);{
            glVertex3d(-1.5, 1.5, 0);
            glVertex3d(-6, 6, 3);
            glVertex3d(-6, -6, 3);
            glVertex3d(-1.5, -1.5, 0);
        }
        glEnd();

        glColor3f(0,0,0);
        glBegin(GL_LINES);{
            glVertex3d(-6, -6, 3);
            glVertex3d(6, -6, 3);
        }
        glEnd();
        glBegin(GL_LINES);{
            glVertex3d(6, -6, 3);
            glVertex3d(6, 6, 3);
        }
        glEnd();
        glBegin(GL_LINES);{
            glVertex3d(6, 6, 3);
            glVertex3d(-6, 6, 3);
        }
        glEnd();
        glBegin(GL_LINES);{
            glVertex3d(-6, 6, 3);
            glVertex3d(-6, -6, 3);
        }
        glEnd();
        glBegin(GL_LINES);{
            glVertex3d(-6, 6, 3);
            glVertex3d(-1.5, 1.5, 0);
        }
        glEnd();
        glBegin(GL_LINES);{
            glVertex3d(-6, -6, 3);
            glVertex3d(-1.5, -1.5, 0);
        }
        glEnd();
        glBegin(GL_LINES);{
            glVertex3d(6, -6, 3);
            glVertex3d(1.5, -1.5, 0);
        }
        glEnd();
        glBegin(GL_LINES);{
            glVertex3d(6, 6, 3);
            glVertex3d(1.5, 1.5, 0);
        }
        glEnd();

        glColor3f(0, 0, 4.5);
        glBegin(GL_POLYGON);{
            glVertex3d(-16, -26, 0);
            glVertex3d(16, -26, 0);
            glVertex3d(26, -16, 0);
            glVertex3d(26, 16, 0);
            glVertex3d(16, 26, 0);
            glVertex3d(-16, 26, 0);
            glVertex3d(-26, 16, 0);
            glVertex3d(-26, -16, 0);

        }glEnd();
}


void faisal_masjid(){
    full_base();

    glPushMatrix();{
        glTranslated(0, 0, -20);
        full_base();
    }glPopMatrix();


    base_side_wall();

    //********************************MINARETS********************************
    four_minarets();

    //*******************************SANCTUARY********************************
    full_sanctuary();

    //*****************************SANCTUARY-TOP******************************
    full_top_structure();

    //****************************ENTRANCE SHADES*****************************
    entrance_full_shade();


    full_side_shades();


    staires();

    glPushMatrix();{
        glTranslated(-225, 0, 0.5);
        water_source();
    }
    glPopMatrix();
}


void display(){
	//codes for Models, Camera

	//clear the display
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glClearColor(3, 3, 3, 1);    //color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        //clear buffers to preset values

	/***************************
	/ set-up camera (view) here
	****************************/
	//load the correct matrix -- MODEL-VIEW matrix

	glMatrixMode(GL_MODELVIEW);        //specify which matrix is the current matrix

	//initialize the matrix

	glLoadIdentity();                //replace the current matrix with the identity matrix [Diagonals have 1, others have 0]

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
    //3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	{

        double x = sqrt(R*R+1);
        double y = atan(1.0/R);

        double a = x*cos(vAngle+y)*cos(hAngle) - R*cos(vAngle)*cos(hAngle);
        double b = x*cos(vAngle+y)*sin(hAngle) - R*cos(vAngle)*sin(hAngle);
        double c = x*sin(vAngle+y) - R*sin(vAngle);

        gluLookAt(R*cos(vAngle)*cos(hAngle),R*cos(vAngle)*sin(hAngle),R*sin(vAngle),  0,0,0,  a,b,c);
        //gluLookAt(eyeX,eyeY,eyeZ,   centerX,centerY,centerZ,    0,0,1);
	}
	//gluLookAt(0,0,200,	0,0,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/**************************************************
	/ Grid and axes Lines(You can remove them if u want)
	***************************************************/

	//drawXYZAxis(1000);//axis length

	//drawGridLines(1000);//grid line length


	/****************************
	/ Add your objects from here
	****************************/
    glTranslatef(0, up_down, 0);
    glTranslatef(left_right, 0, 0);
    glRotatef(rotate_x, 1, 0 ,0);
    glRotatef(rotate_y, 0, 1 ,0);



    /*
	glEnable(GL_TEXTURE_2D);
		{
		    glColor3f(1,1,1);
			glBindTexture(GL_TEXTURE_2D, skyImage);
				glBegin(GL_QUADS);
				{
				    glTexCoord3f(0.0, 0.0, 0.0);
				    glVertex3f(-1000,-1000,-20);

				    glTexCoord3f(1.0, 0.0, 0.0);
				    glVertex3f(1000,-1000,-20);

				    glTexCoord3f(1.0, 1.0, 0.0);
				    glVertex3f(1000,1000,-20);

				    glTexCoord3f(0.0, 1.0, 0.0);
				    glVertex3f(-1000,1000,-20);
				}
				glEnd();
				/*GLUquadricObj *obj = gluNewQuadric();
				gluQuadricTexture(obj, GL_TRUE);
				//gluSphere(obj, 50, 20, 20);
		}
		*/


//    drawAxis();
//    drawGrid();

    //glScaled(2,2,2);

    //glRotated(rotation, 0, 0, 1);

    faisal_masjid();


	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glFlush();
	glutSwapBuffers();
}


void animate(){
	//codes for any changes in Models, Camera

	//cameraAngle += cameraAngleDelta;
	// camera will rotate at 0.001 radians per frame.

	rotation+=0.2;

	//codes for any changes in Models

	//MISSING SOMETHING? -- YES: add the following
	glutPostRedisplay();    //this will call the display AGAIN
}


void init(){
    //codes for initialization

    R = 700.0;
	Rspeed = 10.0;

	vAngle = 0.0;
    hAngle = 0.0;
    angleSpeed = 0.10;

    drawgrid = 0;
    drawaxes = 1;
    cameraHeight = 100.0;
    cameraAngle = 120;
    angle = 0;

    //clear the screen
    glClearColor(0,0,0,0);

    /************************
    / set-up projection here
    ************************/
    //load the PROJECTION matrix
    glMatrixMode(GL_PROJECTION);

    //initialize the matrix
    glLoadIdentity();

    //give PERSPECTIVE parameters
    gluPerspective(80,	1,	1,	1000.0);
    //field of view in the Y (vertically)
    //aspect ratio that determines the field of view in the X direction (horizontally)
    //near distance
    //far distance
}


int main(int argc, char **argv){
    glutInit(&argc,argv);
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

    glutCreateWindow("Faisal_Mashjid");

    printf("************************** Key Function ****************************\n");
    printf("Arrow-Up: Clockwise Rotation along principle X axis;\n");
    printf("Arrow-DownUp: Anti-Clockwise Rotation along principle X axis;\n");
    printf("Arrow-Left: Anti-Clockwise Rotation along principle Z axis;\n");
    printf("Arrow-Right: Clockwise Rotation along principle Z axis;\n");
    printf("Page-Up or z: Zoom-in;\n");
    printf("Page-Down or z: Zoom-out;\n");
    printf("w: Move the object along +Y axis;\n");
    printf("s: Move the object along -Y axis;\n");
    printf("d: Move the object along +X axis;\n");
    printf("a: Move the object along -X axis;\n");
    printf("r: Clockwise Rotation along Y axis;\n");
    printf("f: Anti-Clockwise Rotation along Y axis;\n");
    printf("********************************************************************\n\n");

    init();
    //loadAllImages();
    glEnable(GL_DEPTH_TEST);	//enable Depth Testing

    glutDisplayFunc(display);	//display callback function
    glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutMouseFunc(mouseListener);
    glutMotionFunc(mouseMotion);

    glutMainLoop();		//The main loop of OpenGL

    return 0;
}
