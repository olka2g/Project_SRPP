#include "Visualization.h"

CImgDisplay displ;
CImg<unsigned char> map_image;

struct ColorRGB
{
	unsigned char R;
	unsigned char G;
	unsigned char B;
};

// Given H,S,L in range of 0-1
// Returns a Color (RGB struct) in range of 0-255
ColorRGB HSL2RGB(double h, double sl, double l)
{
	double v;
	double r,g,b;

	r = l;   // default to gray
	g = l;
	b = l;
	v = (l <= 0.5) ? (l * (1.0 + sl)) : (l + sl - l * sl);
	if (v > 0)
	{
		double m;
		double sv;
		int sextant;
		double fract, vsf, mid1, mid2;

		m = l + l - v;
		sv = (v - m ) / v;
		h *= 6.0;
		sextant = (int)h;
		fract = h - sextant;
		vsf = v * sv * fract;
		mid1 = m + vsf;
		mid2 = v - vsf;
		switch (sextant)
		{
		case 0:
			r = v;
			g = mid1;
			b = m;
			break;
		case 1:
			r = mid2;
			g = v;
			b = m;
			break;
		case 2:
			r = m;
			g = v;
			b = mid1;
			break;
		case 3:
			r = m;
			g = mid2;
			b = v;
			break;
		case 4:
			r = mid1;
			g = m;
			b = v;
			break;
		case 5:
			r = v;
			g = m;
			b = mid2;
			break;
		}
	}
	ColorRGB rgb;
	rgb.R = (unsigned char)(r * 255.0f);
	rgb.G = (unsigned char)(g * 255.0f);
	rgb.B = (unsigned char)(b * 255.0f);
	return rgb;
}

void hue_to_rgb(double hue, unsigned char * color)
{
	ColorRGB c = HSL2RGB(hue,1.0,0.5);
	color[0] = c.R;
	color[1] = c.G;
	color[2] = c.B;
}

void getBoundaries(CitiesData &cd, int &maxX, int &minX, int &maxY, int &minY)
{
	minX = INT_MAX,maxX = INT_MIN,minY = INT_MAX,maxY = INT_MIN;

	if(cd.warehouse.location.x > maxX) maxX = cd.warehouse.location.x;
	if(cd.warehouse.location.x < minX) minX = cd.warehouse.location.x;
	if(cd.warehouse.location.y > maxY) maxY = cd.warehouse.location.y;
	if(cd.warehouse.location.y < minY) minY = cd.warehouse.location.y;

	for (int i = 0; i < cd.count; i++)
	{
		if(cd.cities[i].location.x > maxX) maxX = cd.cities[i].location.x;
		if(cd.cities[i].location.x < minX) minX = cd.cities[i].location.x;
		if(cd.cities[i].location.y > maxY) maxY = cd.cities[i].location.y;
		if(cd.cities[i].location.y < minY) minY = cd.cities[i].location.y;
	}

	maxY += 20;
	maxX += 20;
	minY -= 20;
	minX -= 20;	
}

void createCitiesDisplay(CImgDisplay& disp, CImg<unsigned char>& image, CitiesData cd)
{
	int minX,minY,maxX,maxY;
	getBoundaries(cd, maxX, minX, maxY, minY);

	CImg<unsigned char> img(maxY,maxX,1,3,0);	
	unsigned char white[] = { 255,255,255 };
	unsigned char blue[] = { 0,0,255 };

	for (int i = -1; i < cd.count; i++)
	{
		City c;
		unsigned char* color;
		if(i == -1){
			c = cd.warehouse;
			color = blue;
		}else{
			c = cd.cities[i];
			color = white;
		}

		char buf[16];
		sprintf(buf,"%d",c.id);

		img.draw_point(c.location.x,c.location.y,color,1.0);
		img.draw_text(c.location.x,c.location.y+4,buf,color);
	}


	//CImgDisplay main_disp(img,"Map of the area");
	CImgDisplay main_disp;
	//main_disp.wait();

	disp = main_disp;
	image = img;
}

void displayResult(CImgDisplay display, CImg<unsigned char> map_image, Solution solution)
{
	double hue = 0.0;
	unsigned char color [3] = {0,0,0};
	hue_to_rgb(hue, color);
	
	CImg<unsigned char> allMoves = map_image;

	for (int i = 0; i < solution.num_routes; i++)
	{
		Point from, to;
		for (int city = 0; city < solution.routes[i].num_cities - 1; city++)
		{
			from = solution.routes[i].cities[city].location;
			to = solution.routes[i].cities[city+1].location;

			allMoves.draw_line(from.x,from.y,to.x,to.y,color,1.0);			
		}

		hue += (1.0/(double)solution.num_routes);
		hue_to_rgb(hue, color);

	}

	display.display(allMoves);
	
	while(!display.is_closed())display.wait();
}

void displayResultAnimated(CImgDisplay display, CImg<unsigned char> thisRoute, Solution solution)
{
	double hue = 0.0;
	unsigned char color [3] = {0,0,0};
	unsigned char green [3] = {0,255,0};
	unsigned char red [3] = {255,0,0};
	hue_to_rgb(hue, color);

	CImg<unsigned char> cleanMap = thisRoute;
	CImg<unsigned char> allMoves = thisRoute;

	for (int i = 0; i < solution.num_routes; i++)
	{
		Point from, to;
		for (int city = 0; city < solution.routes[i].num_cities - 1; city++)
		{
			from = solution.routes[i].cities[city].location;
			to = solution.routes[i].cities[city+1].location;

			CImg<unsigned char> thisMove = thisRoute;
			thisRoute.draw_line(from.x,from.y,to.x,to.y,color,1.0);
			allMoves.draw_line(from.x,from.y,to.x,to.y,color,1.0);			
			thisMove.draw_arrow(from.x,from.y,to.x,to.y,red,1.0);


			display.display(thisMove);
			Sleep(10);
		}

		display.display(thisRoute);
		thisRoute = cleanMap;

		Sleep(250);

		hue += (1.0/(double)solution.num_routes);
		hue_to_rgb(hue, color);
	}

	display.display(allMoves);
	//all.display(display);
	while(!display.is_closed())display.wait();
}

void createCitiesDisplay(CitiesData cd){
	createCitiesDisplay(displ,map_image, cd);
}

void displayResult(Solution solution){
	displayResult(displ,map_image,solution);
}

void displayResultAnimated(Solution solution){
	displayResultAnimated(displ,map_image,solution);
}