#include <stdio.h>
#include "Renderer.hpp"
int main(void)
{
	printf("###starting\n");
	Renderer r = Renderer(1920, 1080);
	printf("###initializing\n");
	r.initialize();
	printf("###running\n");
	r.run();
	printf("###ending\n");

}