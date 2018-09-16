#ifndef _TRIANGLE_DEMO_H
#define _TRIANGLE_DEMO_H

#include "demo_base.h"
#include <math.h>
#include "lodepng.h"
#include <iostream>

#define TEXTURE_COUNT 2

class TriangleDemo : public DemoBase
{
private:
	float m_rotation = 0;
	float m_value1 = 0;
	float m_textureTimer = 0;

	Vector green = Vector(0.082f, 0.223f, 0.074f);
	Vector darkerGreen = Vector(0.066f, 0.176f, 0.062f);
	Vector black = Vector(0.0f, 0.0f, 0.0f);
	Vector red = Vector(0.4f, 0.0f, 0.0f);
	Vector yellow = Vector(1.0f, 1.0f, 0.0f);

	GLuint mTextureID[TEXTURE_COUNT];

	void loadPNG(const char* path, GLuint textureID)
	{
		//load file and decode image
		std::vector<unsigned char> image;
		unsigned width, height;
		unsigned error = lodepng::decode(image, width, height, path);

		//If there's an error, display it
		if (error != 0)
		{
			std::cout << "png load error :" << error << " : " << lodepng_error_text(error) << std::endl;
			return;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //apply texture wrapping along horizontal part
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //apply texture wrapping along vertical part

		//bilinear filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //near filtering (for when texture needs to be scaled up)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //near filtering (for when texture needs to be scaled down)

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
	}

public:
	void init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_ALPHA_TEST);

		glGenTextures(TEXTURE_COUNT, mTextureID);
		loadPNG("../media/glass.png", mTextureID[0]);
		loadPNG("../media/image.png", mTextureID[1]);
	}

	void deinit()
	{
	}

	void drawAxis(const Matrix& viewMatrix)
	{
	// ============draw axis.
		glLoadMatrixf((GLfloat*)viewMatrix.mVal);
		glBegin(GL_LINES);
			glColor3f(1.0f, 0.3f, 0.3f);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(1.0f, 0.0f, 0.0f);

			glColor3f(0.3f, 1.0f, 0.3f);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 1.0f, 0.0f);

			glColor3f(0.3f, 0.3f, 1.0f);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 0.0f, 1.0f);
		glEnd();
		// ============================
	}

	void DrawTriangle(Vector p0, Vector p1, Vector p2)
	{
		glBegin(GL_TRIANGLES);
		glVertex3f(p0.mVal[0], p0.mVal[1], p0.mVal[2]);
		glVertex3f(p1.mVal[0], p1.mVal[1], p1.mVal[2]);
		glVertex3f(p2.mVal[0], p2.mVal[1], p2.mVal[2]);

		glEnd();
	}
	void DrawCylinder (GLfloat radius, GLfloat taper, GLfloat height, Vector color)
	{
		int i;
		int triangleCount = 50;
		GLfloat angle = (M_2PI / triangleCount);// / (M_DEG_2_RAD);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glBegin(GL_TRIANGLES);
		glColor3f(color.mVal[0], color.mVal[1], color.mVal[2]);
		for (int i = 0; i < triangleCount; i++)
		{
			glVertex3f(height / 2, 0, 0);								//Middle point
			for (int j = 0; j < 2; j++)
			{
				glVertex3f(height / 2, radius*sin(i * angle), radius*cos(i * angle));
				glVertex3f(height / 2, radius*sin((i + 1) * angle), radius*cos((i + 1) * angle));
			}
			glVertex3f(-height / 2, taper*sin(i * angle), taper*cos(i * angle));

			glVertex3f(-height / 2, 0, 0);
			for (int j = 0; j < 2; j++)
			{
				glVertex3f(-height / 2, taper*sin(i * angle), taper*cos(i * angle));
				glVertex3f(-height / 2, taper*sin((i + 1) * angle), taper*cos((i + 1) * angle));
			}
			glVertex3f(height / 2, radius*sin((i + 1) * angle), radius*cos((i+1) * angle));
			
		}

		glEnd();

	}

	void DrawHalfCylinder(GLfloat radius, GLfloat taper, GLfloat height, Vector color)
	{
		int i;
		int triangleCount = 50;
		GLfloat angle = (M_2PI / triangleCount);// / (M_DEG_2_RAD);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glBegin(GL_TRIANGLES);
		glColor3f(color.mVal[0], color.mVal[1], color.mVal[2]);
		for (int i = 0; i < triangleCount/2; i++)
		{
			glVertex3f(height / 2, 0, 0);								//Middle point
			for (int j = 0; j < 2; j++)
			{
				glVertex3f(height / 2, radius*sin(i * angle), radius*cos(i * angle));
				glVertex3f(height / 2, radius*sin((i + 1) * angle), radius*cos((i + 1) * angle));
			}
			glVertex3f(-height / 2, taper*sin(i * angle), taper*cos(i * angle));

			glVertex3f(-height / 2, 0, 0);
			for (int j = 0; j < 2; j++)
			{
				glVertex3f(-height / 2, taper*sin(i * angle), taper*cos(i * angle));
				glVertex3f(-height / 2, taper*sin((i + 1) * angle), taper*cos((i + 1) * angle));
			}
			glVertex3f(height / 2, radius*sin((i + 1) * angle), radius*cos((i + 1) * angle));

		}

		glEnd();

	}

	void DrawSphere(GLfloat ladRadius,GLfloat longRadius, Vector color, GLint resolution)
	{
		GLfloat theta = M_2PI / resolution;
		GLfloat alpha = M_PI / resolution;

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glColor3f(color.mVal[0], color.mVal[1], color.mVal[2]);

		for (int i = 0; i < resolution; i++)
		{
			for (int j = 0; j < resolution; j++)
			{
				float t1 = i * theta;
				float a1 = j * alpha;
				float t2 = (i + 1) * theta;
				float a2 = (j + 1) * alpha;

				Vector p0 = Vector (ladRadius * cos(t1) * sin(a1), longRadius * cos(a1), ladRadius * sin (t1) * sin (a1));
				Vector p1 = Vector (ladRadius * cos(t1) * sin(a2), longRadius * cos(a2), ladRadius * sin (t1) * sin (a2));
				Vector p2 = Vector (ladRadius * cos(t2) * sin(a1), longRadius * cos(a1), ladRadius * sin (t2) * sin (a1));
				Vector p3 = Vector (ladRadius * cos(t2) * sin(a2), longRadius * cos(a2), ladRadius * sin (t2) * sin (a2));

				DrawTriangle(p0, p2, p1);
				DrawTriangle(p3, p1, p2);
			}
		}
		
	}

	void DrawHalfSphere(GLfloat ladRadius, GLfloat longRadius, Vector color, GLint resolution)
	{
		GLfloat theta = M_2PI / resolution;
		GLfloat alpha = M_PI / resolution;

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glColor3f(color.mVal[0], color.mVal[1], color.mVal[2]);

		for (int i = 0; i < resolution; i++)
		{
			for (int j = 0; j < resolution/2; j++)
			{
				float t1 = i * theta;
				float a1 = j * alpha;
				float t2 = (i + 1) * theta;
				float a2 = (j + 1) * alpha;

				Vector p0 = Vector(ladRadius * cos(t1) * sin(a1), longRadius * cos(a1), ladRadius * sin(t1) * sin(a1));
				Vector p1 = Vector(ladRadius * cos(t1) * sin(a2), longRadius * cos(a2), ladRadius * sin(t1) * sin(a2));
				Vector p2 = Vector(ladRadius * cos(t2) * sin(a1), longRadius * cos(a1), ladRadius * sin(t2) * sin(a1));
				Vector p3 = Vector(ladRadius * cos(t2) * sin(a2), longRadius * cos(a2), ladRadius * sin(t2) * sin(a2));

				DrawTriangle(p0, p2, p1);
				DrawTriangle(p3, p1, p2);
			}
		}

	}

	void DrawCuboid(GLfloat length, GLfloat depth, GLfloat height,Vector position, Vector color)
	{
		float xExtent = length / 2;
		float yExtent = height / 2;
		float zExtent = depth / 2;

		Vector xyz		= Vector(position.mVal[0] - xExtent, position.mVal[1] - yExtent, position.mVal[2] - zExtent);
		Vector xy1z		= Vector(position.mVal[0] - xExtent, position.mVal[1] + yExtent, position.mVal[2] - zExtent);
		Vector xy1z1	= Vector(position.mVal[0] - xExtent, position.mVal[1] + yExtent, position.mVal[2] + zExtent);
		Vector xyz1		= Vector(position.mVal[0] - xExtent, position.mVal[1] - yExtent, position.mVal[2] + zExtent);
		Vector x1yz		= Vector(position.mVal[0] + xExtent, position.mVal[1] - yExtent, position.mVal[2] - zExtent);
		Vector x1y1z	= Vector(position.mVal[0] + xExtent, position.mVal[1] + yExtent, position.mVal[2] - zExtent);
		Vector x1y1z1	= Vector(position.mVal[0] + xExtent, position.mVal[1] + yExtent, position.mVal[2] + zExtent);
		Vector x1yz1	= Vector(position.mVal[0] + xExtent, position.mVal[1] - yExtent, position.mVal[2] + zExtent);

		glColor3f(color.mVal[0], color.mVal[1], color.mVal[2]);
		//Face 1
		DrawTriangle(xyz, x1yz, xy1z);
		DrawTriangle(xy1z, x1y1z, x1yz);

		//Face 2
		DrawTriangle(x1y1z, x1yz, x1yz1);
		DrawTriangle(x1y1z, x1yz1, x1y1z1);

		//Face 3
		DrawTriangle(x1y1z1, x1yz1, xyz1);
		DrawTriangle(xyz1, xy1z1, x1y1z1);

		//Face 4
		DrawTriangle(xy1z1, xyz1, xyz);
		DrawTriangle(xyz, xy1z, xy1z1);

		//Face 5
		DrawTriangle(xy1z1, xy1z, x1y1z);
		DrawTriangle(x1y1z, x1y1z1, xy1z1);

		//Face 6
		DrawTriangle(xyz1, x1yz1, xyz);
		DrawTriangle(xyz, x1yz, x1yz1);
	}
	void DrawFins(GLfloat length, GLfloat height, GLfloat offset)
	{
		//Red Part
		DrawCuboid(length * 0.7f, 0.05f, height, Vector(0, offset + height /2 , 0), red);
		
		Vector p0 = Vector(-(length * 0.7f) / 2 , offset		 ,  0.025f);
		Vector p1 = Vector(-(length * 0.7f) / 2 , offset + height,  0.025f);
		Vector p2 = Vector(-length / 2			, offset		 ,  0.025f);
		Vector p3 = Vector(-length / 2			, offset		 , -0.025f);
		Vector p4 = Vector(-(length * 0.7f) / 2 , offset + height, -0.025f);
		Vector p5 = Vector(-(length * 0.7f) / 2 , offset		 , -0.025f);

		DrawTriangle(p0, p1, p2);
		DrawTriangle(p2, p3, p1);
		DrawTriangle(p1, p3, p4);
		DrawTriangle(p3, p4, p5);
		DrawTriangle(p0, p1, p5);
		DrawTriangle(p1, p4, p5);
		DrawTriangle(p0, p2, p5);
		DrawTriangle(p2, p3, p5);

		p0 = Vector((length * 0.7f) / 2, offset			,  0.025f);
		p1 = Vector((length * 0.7f) / 2, offset + height,  0.025f);
		p2 = Vector(length / 2		   , offset			,  0.025f);
		p3 = Vector(length / 2		   , offset			, -0.025f);
		p4 = Vector((length * 0.7f) / 2, offset + height, -0.025f);
		p5 = Vector((length * 0.7f) / 2, offset			, -0.025f);

		DrawTriangle(p0, p1, p2);
		DrawTriangle(p2, p3, p1);
		DrawTriangle(p1, p3, p4);
		DrawTriangle(p3, p4, p5);
		DrawTriangle(p0, p1, p5);
		DrawTriangle(p1, p4, p5);
		DrawTriangle(p0, p2, p5);
		DrawTriangle(p2, p3, p5);

		//Green part side 1
		DrawCuboid(length * 0.65f, 0.025f, height * 0.85f, Vector(0, offset + height * 0.85f / 2, 0.025f), green);
		
		p0 = Vector(-(length * 0.65f) / 2, offset				  , 0.04f);
		p1 = Vector(-(length * 0.65f) / 2, offset + height * 0.85f, 0.04f);
		p2 = Vector(-(length * 0.85f) / 2, offset				  , 0.04f);
		p3 = Vector(-(length * 0.85f) / 2, offset				  , 0.025f);
		p4 = Vector(-(length * 0.65f) / 2, offset + height * 0.85f, 0.025f);
		p5 = Vector(-(length * 0.65f) / 2, offset				  , 0.025f);

		DrawTriangle(p0, p1, p2);
		DrawTriangle(p2, p3, p1);
		DrawTriangle(p1, p3, p4);
		DrawTriangle(p3, p4, p5);
		DrawTriangle(p0, p1, p5);
		DrawTriangle(p1, p4, p5);
		DrawTriangle(p0, p2, p5);
		DrawTriangle(p2, p3, p5);

		p0 = Vector((length * 0.65f) / 2, offset, 0.04f);
		p1 = Vector((length * 0.65f) / 2, offset + height * 0.85f, 0.04f);
		p2 = Vector((length * 0.85f) / 2, offset, 0.04f);
		p3 = Vector((length * 0.85f) / 2, offset, 0.025f);
		p4 = Vector((length * 0.65f) / 2, offset + height * 0.85f, 0.025f);
		p5 = Vector((length * 0.65f) / 2, offset, 0.025f);

		DrawTriangle(p0, p1, p2);
		DrawTriangle(p2, p3, p1);
		DrawTriangle(p1, p3, p4);
		DrawTriangle(p3, p4, p5);
		DrawTriangle(p0, p1, p5);
		DrawTriangle(p1, p4, p5);
		DrawTriangle(p0, p2, p5);
		DrawTriangle(p2, p3, p5);

		//Green Part Side 2
		DrawCuboid(length * 0.65f, 0.025f, height * 0.85f, Vector(0, offset + height * 0.85f / 2, -0.025f), green);

		p0 = Vector(-(length * 0.65f) / 2, offset, -0.04f);
		p1 = Vector(-(length * 0.65f) / 2, offset + height * 0.85f, -0.04f);
		p2 = Vector(-(length * 0.85f) / 2, offset, -0.04f);
		p3 = Vector(-(length * 0.85f) / 2, offset, -0.025f);
		p4 = Vector(-(length * 0.65f) / 2, offset + height * 0.85f, -0.025f);
		p5 = Vector(-(length * 0.65f) / 2, offset, -0.025f);

		DrawTriangle(p0, p1, p2);
		DrawTriangle(p2, p3, p1);
		DrawTriangle(p1, p3, p4);
		DrawTriangle(p3, p4, p5);
		DrawTriangle(p0, p1, p5);
		DrawTriangle(p1, p4, p5);
		DrawTriangle(p0, p2, p5);
		DrawTriangle(p2, p3, p5);

		p0 = Vector((length * 0.65f) / 2,offset, -0.04f);
		p1 = Vector((length * 0.65f) / 2,offset + height * 0.85f, -0.04f);
		p2 = Vector((length * 0.85f) / 2,offset, -0.04f);
		p3 = Vector((length * 0.85f) / 2,offset, -0.025f);
		p4 = Vector((length * 0.65f) / 2,offset + height * 0.85f, -0.025f);
		p5 = Vector((length * 0.65f) / 2,offset, -0.025f);

		DrawTriangle(p0, p1, p2);
		DrawTriangle(p2, p3, p1);
		DrawTriangle(p1, p3, p4);
		DrawTriangle(p3, p4, p5);
		DrawTriangle(p0, p1, p5);
		DrawTriangle(p1, p4, p5);
		DrawTriangle(p0, p2, p5);
		DrawTriangle(p2, p3, p5);
		
	}
	void DrawBrows(GLfloat length, GLfloat thickness, GLfloat height, Vector color)
	{
		float horizontalSpace = (length / 2) - thickness;
		float verticalSpace = height - thickness;

		glColor3f(color.mVal[0], color.mVal[1], color.mVal[2]);

		for (int i = 0; i < 2; i++)
		{
			Vector p1 = Vector(-horizontalSpace * (1 + (-2 * i)), 0, 0);
			Vector p2 = Vector(0, verticalSpace, 0);
			Vector p3 = Vector(0, height, 0);
			Vector p4 = Vector((-length / 2) * (1 + (-2 * i)), 0, 0);

			Vector p5 = Vector(-horizontalSpace * (1 + (-2 * i)), 0, -thickness / 4);
			Vector p6 = Vector(0, verticalSpace, -thickness / 4);
			Vector p7 = Vector(0, height, -thickness / 4);
			Vector p8 = Vector((-length / 2) * (1 + (-2 * i)), 0, -thickness / 4);

			DrawTriangle(p1, p2, p3);
			DrawTriangle(p1, p3, p4);

			DrawTriangle(p5, p6, p7);
			DrawTriangle(p5, p7, p8);

			DrawTriangle(p4, p3, p7);
			DrawTriangle(p8, p4, p7);

			DrawTriangle(p1, p2, p6);
			DrawTriangle(p1, p5, p6);
		}
	}

	void DrawHeadFins(GLfloat length, GLfloat thickness, GLfloat height, Vector color)
	{
		float horizontalSpace = (length / 2) - thickness;
		float verticalSpace = height - thickness;

		glColor3f(color.mVal[0], color.mVal[1], color.mVal[2]);

		Vector p1 = Vector(horizontalSpace, thickness/2, 0);
		Vector p2 = Vector(length + length / 2, height * 2, 0);
		Vector p3 = Vector(length + horizontalSpace + thickness/2, verticalSpace + height, 0);
		Vector p4 = Vector(length / 2, 0, 0);

		Vector p5 = Vector(horizontalSpace, thickness / 2, -thickness/4);
		Vector p6 = Vector(length * 1.5f, height * 2, -thickness / 4);
		Vector p7 = Vector(length + horizontalSpace + thickness / 2, verticalSpace + height, -thickness / 4);
		Vector p8 = Vector(length / 2, 0, -thickness / 4);

		Vector p9 = Vector(length, verticalSpace, 0);
		Vector p10 = Vector(length / 2 + horizontalSpace, height - thickness/2, 0);

		Vector p11 = Vector(length, verticalSpace, -thickness / 4);
		Vector p12 = Vector(length / 2 + horizontalSpace, height - thickness / 2, -thickness / 4);

		DrawTriangle(p1, p2, p3);
		DrawTriangle(p1, p3, p4);

		DrawTriangle(p5, p6, p7);
		DrawTriangle(p5, p7, p8);

		DrawTriangle(p4, p3, p7);
		DrawTriangle(p8, p4, p7);

		DrawTriangle(p1, p2, p6);
		DrawTriangle(p1, p5, p6);

		DrawTriangle(p3, p9, p10);
		DrawTriangle(p7, p11, p12);

		DrawTriangle(p3, p7, p9);
		DrawTriangle(p7, p11, p9);
		
		DrawTriangle(p11, p9, p10);
		DrawTriangle(p11, p12, p10);
	}

	void draw(const Matrix& viewMatrix)
	{
		drawAxis(viewMatrix);
		m_rotation += 0.05;
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//Tail
		Matrix defaultTranslate = Matrix::makeTranslationMatrix(Vector(0, 0, 10));
		Matrix aRotate1 = Matrix::makeRotateMatrix(sinf(m_rotation * 0.5f) * 15.0f, Vector(1, 0, 0));
		Matrix rotate1 = Matrix::makeRotateMatrix(-3.0f * m_rotation, Vector(0, 1, 0));
		Matrix rotate2;
		Matrix rotate3;
		Matrix translate1;
		Matrix modelMatrix = rotate1 * defaultTranslate;
		Matrix viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);

		DrawCylinder(0.26f, 0.05f, 2.0f, darkerGreen);

		translate1 = Matrix::makeTranslationMatrix(1.001f, 0, 0);
		modelMatrix = rotate1 * defaultTranslate * translate1;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		DrawCylinder(0.255f, 0.255f, 0.02f, black);

		for (int i = 0; i < 2; i++)
		{
			rotate1 = Matrix::makeRotateMatrix((-3.0f * m_rotation), Vector(0, 1, 0));
			rotate2 = Matrix::makeRotateMatrix((180 * i),Vector (1,0,0)) * Matrix::makeRotateMatrix(-6, Vector(0, 0, 1));
			modelMatrix = rotate1 * defaultTranslate * rotate2;
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);

			glColor3f(red.mVal[0], red.mVal[1], red.mVal[2]);
			Vector xyz1 = Vector(0.9f, 0.14f, 0.025f);
			Vector x1yz1 = Vector(-0.9f, 0.14f, 0.025f);
			Vector x1y1z1 = Vector(-0.9, 0.64f, 0.025f);

			Vector xyz = Vector(0.9f,  0.14f, -0.025f);
			Vector x1yz = Vector(-0.9f,  0.14f, -0.025f);
			Vector x1y1z = Vector(-0.9f, 0.64f, -0.025f);

			DrawTriangle(xyz1, x1yz1, x1y1z1);
			DrawTriangle(xyz1, xyz, x1y1z1);
			DrawTriangle(xyz, x1y1z, x1y1z1);
			DrawTriangle(xyz, x1yz, x1y1z);
			DrawTriangle(x1y1z1, x1yz1, x1y1z);
			DrawTriangle(x1y1z, x1yz, x1yz1);

			glColor3f(green.mVal[0], green.mVal[1], green.mVal[2]);
			xyz1  = Vector( 0.7f, 0.1f,  0.026f);
			x1yz1 = Vector(-0.7f, 0.1f,  0.026f);
			x1y1z1= Vector(-0.7,  0.5f,  0.026f);
							  
			xyz   = Vector( 0.7f, 0.1f, -0.026f);
			x1yz  = Vector(-0.7f, 0.1f, -0.026f);
			x1y1z = Vector(-0.7f, 0.5f, -0.026f);

			DrawTriangle(xyz1, x1yz1, x1y1z1);
			DrawTriangle(xyz, x1yz, x1y1z);
		}
		//Smooth Section 1

		for (int i = 0; i < 10; i++)
		{
			rotate1 = Matrix::makeRotateMatrix(-6.5f + (-2.0f * i) + (-3.0f * m_rotation), Vector(0, 1, 0));
			translate1 = Matrix::makeTranslationMatrix(0, 0, 10.05f);
			modelMatrix = rotate1 * translate1;
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			DrawCylinder(0.25f + ((i + 1) * 0.025f), 0.25f + (i * 0.025f), 0.4f, green);
		}

		//Finned Section 1
		rotate1 = Matrix::makeRotateMatrix(-31.0f + (-3.0f * m_rotation), Vector(0, 1, 0));
		modelMatrix = rotate1 * defaultTranslate * aRotate1;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		DrawCylinder(0.53f, 0.53f, 2.0f, darkerGreen);

		for (int i = 0; i < 4; i++)
		{
			rotate2 = Matrix::makeRotateMatrix(90 * i + 45, Vector(1.0f, 0.0f, 0.0f));
			modelMatrix = rotate1 *defaultTranslate  * aRotate1 * rotate2;
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			DrawFins(1.8f, 0.5f, 0.5f);
		}

		for (int i = 0; i < 2; i++)
		{
			translate1 = Matrix::makeTranslationMatrix(-1.001f + (2 * i), 0, 0);
			modelMatrix = rotate1 * defaultTranslate * aRotate1 * translate1;
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			DrawCylinder(0.51f, 0.51f, 0.02f, black);
		}
		//Smooth Section 2
		for (int i = 0; i < 10; i++)
		{
			rotate1 = Matrix::makeRotateMatrix(-37.5f + (-2.0f * i) + (-3.0f * m_rotation), Vector(0, 1, 0));
			translate1 = Matrix::makeTranslationMatrix(0, 0, 10.05f);
			modelMatrix = rotate1 * translate1;
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			DrawCylinder(0.5f, 0.5f, 0.4f, green);
		}

		//Finned Section 2
		rotate1 = Matrix::makeRotateMatrix(-62.0f + (-3.0f * m_rotation), Vector(0, 1, 0));
		modelMatrix = rotate1 * defaultTranslate * aRotate1;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		DrawCylinder(0.53f, 0.53f, 2.0f, darkerGreen);

		for (int i = 0; i < 4; i++)
		{
			rotate2 = Matrix::makeRotateMatrix(90 * i + 45, Vector(1.0f, 0.0f, 0.0f));
			modelMatrix = rotate1 *defaultTranslate * aRotate1 * rotate2;
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			DrawFins(1.8f, 0.5f, 0.5f);
		}

		for (int i = 0; i < 2; i++)
		{
			translate1 = Matrix::makeTranslationMatrix(-1.001f + (2 * i), 0, 0);
			modelMatrix = rotate1 * defaultTranslate * translate1;
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			DrawCylinder(0.51f, 0.51f, 0.02f, black);
		}

		//Smooth Section 3
		for (int i = 0; i < 10; i++)
		{
			rotate1 = Matrix::makeRotateMatrix(-68.5f + (-2.0f * i) + (-3.0f * m_rotation), Vector(0, 1, 0));
			translate1 = Matrix::makeTranslationMatrix(0, 0, 10.05f);
			modelMatrix = rotate1 * translate1;
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			DrawCylinder(0.5f, 0.5f, 0.4f, green);
		}

		//Finned Section 3
		rotate1 = Matrix::makeRotateMatrix(-93.0f + (-3.0f * m_rotation), Vector(0, 1, 0));
		modelMatrix = rotate1 * defaultTranslate * aRotate1;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		DrawCylinder(0.53f, 0.53f, 2.0f, darkerGreen);

		for (int i = 0; i < 4; i++)
		{
			rotate2 = Matrix::makeRotateMatrix(90 * i + 45, Vector(1.0f, 0.0f, 0.0f));
			modelMatrix = rotate1 *defaultTranslate * aRotate1 * rotate2;
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			DrawFins(1.8f, 0.5f, 0.5f);
		}

		for (int i = 0; i < 2; i++)
		{
			translate1 = Matrix::makeTranslationMatrix(-1.001f + (2 * i), 0, 0);
			modelMatrix = rotate1 * defaultTranslate * translate1;
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			DrawCylinder(0.51f, 0.51f, 0.02f, black);
		}

		//Smooth Section 4
		for (int i = 0; i < 10; i++)
		{
			rotate1 = Matrix::makeRotateMatrix(-99.5f + (-2.0f * i) + (-3.0f * m_rotation), Vector(0, 1, 0));
			translate1 = Matrix::makeTranslationMatrix(0, 0, 10.05f);
			modelMatrix = rotate1 * translate1;
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			DrawCylinder(0.5f, 0.5f, 0.4f, green);
		}

		//Finned Section 4
		rotate1 = Matrix::makeRotateMatrix(-124.0f + (-3.0f * m_rotation), Vector(0, 1, 0));
		modelMatrix = rotate1 * defaultTranslate;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		DrawCylinder(0.53f, 0.53f, 2.0f, darkerGreen);

		for (int i = 0; i < 2; i++)
		{
			rotate2 = Matrix::makeRotateMatrix(90 * i - 45, Vector(1.0f, 0.0f, 0.0f));
			modelMatrix = rotate1 *defaultTranslate * rotate2;
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			DrawFins(1.8f, 0.5f, 0.5f);
		}

		for (int i = 0; i < 2; i++) //hands
		{
			translate1 = Matrix::makeTranslationMatrix(0, 0.55f, 0);
			rotate2 = Matrix::makeRotateMatrix(120 * (1 + i), Vector(1, 0, 0));
			modelMatrix = rotate1 * defaultTranslate * rotate2 * translate1;
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			DrawSphere(0.2f, 0.2f, darkerGreen, 50);

			translate1 = Matrix::makeTranslationMatrix(0, 0.35f, 0);
			modelMatrix = rotate1 * defaultTranslate * rotate2 * translate1 * Matrix::makeRotateMatrix(90, Vector(0, 0, 1));
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			DrawCylinder(0.25f, 0.25f, 0.4, black);

			translate1 = Matrix::makeTranslationMatrix(0, 1.0f, 0);
			modelMatrix = rotate1 * defaultTranslate * rotate2 *  translate1 * Matrix::makeRotateMatrix(90, Vector(0, 0, 1));
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			DrawCylinder(0.08f, 0.08f, 0.55f, darkerGreen);

			translate1 = Matrix::makeTranslationMatrix(0, 1.3f, 0);
			modelMatrix = rotate1 * defaultTranslate * rotate2 *  translate1;
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			DrawSphere(0.08f, 0.08f, black, 50);

			translate1 = Matrix::makeTranslationMatrix(0, 1.55f, -0.15f * (1 - i * 2));
			modelMatrix = rotate1 * defaultTranslate * rotate2 *  translate1
				* Matrix::makeRotateMatrix(30 * (1 - i * 2), Vector(1, 0, 0))
				* Matrix::makeRotateMatrix(90, Vector(0, 0, 1));
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			DrawCylinder(0.08f, 0.08f, 0.55f, darkerGreen);

		}
		

		for (int i = 0; i < 2; i++)
		{
			translate1 = Matrix::makeTranslationMatrix(-1.001f + (2 * i), 0, 0);
			modelMatrix = rotate1 * defaultTranslate * translate1;
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			DrawCylinder(0.51f, 0.51f, 0.02f, black);
		}

		//Smooth Section 5
		for (int i = 0; i < 10; i++)
		{
			rotate1 = Matrix::makeRotateMatrix(-130.5f + (-2.0f * i) + (-3.0f * m_rotation), Vector(0, 1, 0));
			translate1 = Matrix::makeTranslationMatrix(0, 0, 10.05f);
			modelMatrix = rotate1 * translate1;
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			DrawCylinder(0.5f, 0.5f, 0.4f, green);
		}

		//Headdo
		for (int i = 0; i < 2; i++)
		{
			rotate1 = Matrix::makeRotateMatrix(-155.5f + (-3.0f * m_rotation), Vector(0, 1, 0));
			rotate2 = Matrix::makeRotateMatrix(180 * i, Vector(1, 0, 0));
			rotate3 = Matrix::makeRotateMatrix(18 * i, Vector(0, 0, 1));
			translate1 = Matrix::makeTranslationMatrix(Vector(-0.1 * i, -0.3 * i, 0));

			modelMatrix = rotate1 * defaultTranslate * translate1 * rotate3 * rotate2;
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			DrawHalfCylinder(0.05f, 0.5f, 2.2f, green);

			glColor3f(red.mVal[0], red.mVal[1], red.mVal[2]);
			DrawTriangle(Vector(1.1f, 0, -0.05f), Vector( 1.1f, 0, 0.05f), Vector(-1.1f, 0, -0.5f));
			DrawTriangle(Vector(1.1f, 0,  0.05f), Vector(-1.1f, 0, 0.5f ), Vector(-1.1f, 0, -0.5f));
		}

		//eyes
		for (int i = 0; i < 2; i++)
		{
			rotate1 = Matrix::makeRotateMatrix(-155.5f + (-3.0f * m_rotation), Vector(0, 1, 0));
			translate1 = Matrix::makeTranslationMatrix(0, 0.3f, 0);
			rotate2 = Matrix::makeRotateMatrix(180 + 120 * (1 + i), Vector(1, 0, 0));
			modelMatrix = rotate1 * defaultTranslate * rotate2 * translate1;
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			DrawSphere(0.07f, 0.07f, black, 50);

			translate1 = Matrix::makeTranslationMatrix(0, 0.34f, 0);
			modelMatrix = rotate1 * defaultTranslate * rotate2 * translate1;
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			DrawSphere(0.04f, 0.04f, yellow, 50);

			translate1 = Matrix::makeTranslationMatrix(0.04f, 0.34f, 0);
			rotate1 = Matrix::makeRotateMatrix(-154.6f + (-3.0f * m_rotation), Vector(0, 1, 0));
			rotate3 = Matrix::makeRotateMatrix(10, Vector(0, 0, 1));
			modelMatrix = rotate1 * defaultTranslate * rotate2 * translate1 * rotate3;
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			DrawHalfCylinder(0.07f, 0.01f, 0.4f, black);
		}

		//brows
		for (int i = 0; i < 2; i++)
		{
			rotate1 = Matrix::makeRotateMatrix(-154.1f + (-3.0f * m_rotation), Vector(0, 1, 0));
			rotate2 = Matrix::makeRotateMatrix(195 * (1 + (-2 * i)) + 5 * (i - 1), Vector(0, 1, 0));
			rotate3 = Matrix::makeRotateMatrix(-30 * (1 + (-2 * i)), Vector(1, 0, 0));
			translate1 = Matrix::makeTranslationMatrix(Vector(0.05f, 0.07f, -0.30f * (1 + (-2 * i))));
			modelMatrix = rotate1 * defaultTranslate * translate1 * rotate2;
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);

			DrawBrows(2.5f, 0.2f, 0.4f, green);

			modelMatrix = rotate1 * defaultTranslate * rotate2 * rotate3 *
				Matrix::makeRotateMatrix(-30, Vector(0, 0, 1)) * Matrix::makeTranslationMatrix(Vector(-0.5f, -0.16f, 0));
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			DrawHeadFins(3.0f, 0.3f, 0.6f, green);

			rotate3 = Matrix::makeRotateMatrix(-60 * (1 + (-2 * i)) + 180, Vector(1, 0, 0));
			modelMatrix = rotate1 * defaultTranslate * rotate2 * rotate3 *
				Matrix::makeRotateMatrix(-30, Vector(0, 0, 1)) * Matrix::makeTranslationMatrix(Vector(0.3f, 0.16f, 0));
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			DrawHeadFins(1.0f, 0.2f, 0.4f, green);
		}

	}
};

#endif
