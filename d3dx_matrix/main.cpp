#include <cstdio>
#include "d3dx9math.h"


static void PrintMatrix(const char* name, const D3DXMATRIX* m)
{
	char upperName[10000];
	sprintf(upperName, "ref%s", name);
	upperName[3] = toupper(upperName[3]);
	const float* p = reinterpret_cast<const float*>(m);
	printf("static const Matrix %s(\n", upperName);
	for(int i = 0; i < 4; i++)
	{
		if (i != 3)
			printf("\t%+02.6ff, %+02.6ff, %+02.6ff, %+02.6ff,\n", p[0], p[1], p[2], p[3]);
		else
			printf("\t%+02.6ff, %+02.6ff, %+02.6ff, %+02.6ff);\n", p[0], p[1], p[2], p[3]);
		p += 4;
	}
}


int main(int argc, char** argv)
{
	D3DXMATRIX view;
	D3DXMatrixLookAtLH(&view, &D3DXVECTOR3(1, 2, 3), &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 1, 0));
	PrintMatrix("view", &view);

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DXToRadian(80.0f), 1.5f, 1.0f, 100.0f);
	PrintMatrix("proj", &proj);

	D3DXMATRIX viewProj;
	D3DXMatrixMultiply(&viewProj, &view, &proj);
	PrintMatrix("viewProj", &viewProj);

	D3DXMATRIX viewInv;
	float det = -1.0f;
	D3DXMatrixInverse(&viewInv, &det, &view);
	PrintMatrix("viewInv", &viewInv);

	D3DXMATRIX ortho;
	D3DXMatrixOrthoOffCenterLH(&ortho, -2.0f, 1.0f, -1.0f, 2.0f, 0.0f, 1000.0f);
	PrintMatrix("ortho", &ortho);

	return 0;
}