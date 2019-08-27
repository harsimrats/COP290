FILE *f;

void print_transform(float a, float b)
{
    fprintf(f, " transform=\"translate(%f, %f)\">\n", a, b);
}

void print_transform_matrix(float a, float b, float c, float d, float e, float f1)
{
    fprintf(f, " transform=\"matrix(%f, %f, %f, %f, %f, %f)\">\n", a, b, c, d, e, f1);
}

void print_rect(float y, float x, float h, float w, char* fill, char* str, float sw)
{
	fprintf(f, "<rect\n\t\t\ty=\"%f\"\n\t\t\tx=\"%f\"\n\t\t\theight=\"%f\"\n\t\t\twidth=\"%f\"\n\t\t\tstyle=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n", y, x, h, w, fill, str, sw);
}

void print_path(char* d, char* fill, char* str, float sw)
{
	fprintf(f, "<path\n\t\td=\"%s\"\n\t\tstyle=\"fill:%s;stroke:%s;stroke-width:%f\" />\n", d, fill, str, sw);
}

void print_text(float y, float x, float fs, char* ta, char* fill, char* str, float sw, float ty, float tx, char* nam)
{
	fprintf(f, "<text\n\t\t\ty=\"%f\"\n\t\t\tx=\"%f\"\n\t\t\tstyle=\"font-size:%fpx;text-anchor:%s;fill:%s;stroke:%s;stroke-width:%f\"><tspan y=\"%f\" x=\"%f\">%s</tspan></text>\n", y, x, fs, ta, fill, str, sw, ty, tx, nam);
}

void print_text_transform(float y, float x, float fs, char* ta, char* fill, char* str, float sw, float a, float b, float c, float d, float e, float f1, float ty, float tx, char* nam)
{
	fprintf(f, "<text\n\t\t\ty=\"%f\"\n\t\t\tx=\"%f\"\n\t\t\tstyle=\"font-size:%fpx;text-anchor:%s;fill:%s;stroke:%s;stroke-width:%f\"\n", y, x, fs, ta, fill, str, sw);
	print_transform_matrix(a,b,c,d,e,f1);
	fprintf(f, "<tspan y=\"%f\" x=\"%f\">%s</tspan></text>\n", ty, tx, nam);
}

void print_text2(float y, float x, float fs, char* ta, char* fill, char* str, float sw, float ty, float tx, char* nam, float ty1, float tx1, char* nam1)
{
	fprintf(f, "<text\n\t\t\ty=\"%f\"\n\t\t\tx=\"%f\"\n\t\t\tstyle=\"font-size:%fpx;text-anchor:%s;fill:%s;stroke:%s;stroke-width:%f\"><tspan y=\"%f\" x=\"%f\">%s</tspan>\n\t\t\t<tspan y=\"%f\" x=\"%f\">%s</tspan></text>\n", y, x, fs,ta, fill, str, sw, ty, tx, nam, ty1, tx1, nam1);
}

void print_use(float x, float y, float t1, float t2, float w, float h)
{
	fprintf(f, "<use x=\"%f\" y=\"%f\" xlink:href=\"#path2444\" transform=\"translate(%f,%f)\" width=\"%f\" height=\"%f\" />", x, y, t1, t2, w, h);
}

void print_use_transform(float x, float y, float t1, float t2, float t3, float t4, float t5, float t6, float w, float h)
{
	fprintf(f, "<use x=\"%f\" y=\"%f\" xlink:href=\"#path2444\" transform=\"matrix(%f,%f,%f,%f,%f,%f)\" width=\"%f\" height=\"%f\" />", x, y, t1, t2, t3, t4, t5, t6, w, h);
}

void create_svg(int a, int b, int c, int d)
{
    char *c1, *c2, *c3, *c4;
    c1 = malloc(10);
    c2 = malloc(10);
    c3 = malloc(10);
    c4 = malloc(10);
    if(a == 1)
    {
        char* x = "#00ff00";
        c1 = x;
    }
    else c1 = "#ffffff";
    if(b == 1)
    {
        char* x = "#ffff00";
        c2 = x;
    }
    else c2 = "#ffffff";
    if(c == 1)
    {
        char* x = "#00ffff";
        c3 = x;
    }
    else c3 = "#ffffff";
    if(d == 1)
    {
        char* x = "#0000ff";
        c4 = x;
    }
    else c4 = "#ffffff";
    
	f = fopen("a.svg", "w");
	fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n<svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" width=\"800\" height=\"500\">\n\t<g>\n\t\t<g");
	print_transform(0, 75);
	print_rect(175, 25, 100, 25, "#ff0000","#000000", 2);
	print_path("M 25,275 L 37.5,255 L 50,275", "none", "#000000", 2);
	print_text(229.36816, 29.425781, 12, "start","#000000", "none", 1, 229.36816, 29.425781, "PC");
	fprintf(f, "\t\t</g>\n\t\t<g ");
	print_transform(-15, 75);
	print_rect(175, 100, 100, 75, "#ff0000", "#000000", 2);
	print_text(228.12598, 112.67969, 12, "start","#000000", "none", 1, 228.12598, 112.67969, "Memory");
    fprintf(f, "\t\t</g>\n");
	print_path("M 100,100 L 130,115 L 130,155 L 100,170 L 100,140 L 110,135 L 100,130 L 100,100 z", "#ff0000", "#000000", 2);
	print_text(90, 284.5, 12, "middle","none", "#000000", 1, 90, 284.5, "");
	print_text_transform(-116.75, 132.88281, 12, "middle", "#000000", "none", 1, 0, 1, -1, 0, 0, 0, -116.75, 132.88281, "Adder");
	print_rect(155.10001, 265, 94.900002, 59.716309, c1, "#000000", 1.99999988);
	print_text2(196.97363, 294.27515, 12, "middle", "#000000", "none", 1, 196.97363, 294.27515, "Register", 211.97363, 294.27515, "File");
	print_text(235, 334.125, 12, "start", "none", "#000000", 1, 235, 334.125, "");
	fprintf(f, "\t\t<g ");
	print_transform(-37.747, 80);
	fprintf(f, "<path transform=\"matrix(1.0016064,0,0,1,-20.52208,22.5)\" d=\"M 375,237.5 A 25,37.5 0 1 1 325,237.5 A 25,37.5 0 1 1 375,237.5 z\" style=\"fill:%s;stroke:#000000;stroke-width:1.99839556\" />", c1);
	print_text2(256.97363, 329.99622, 12, "middle", "#000000", "none", 1, 256.97363, 329.99622, "Sign", 271.97363, 329.99622, "Extend");
	fprintf(f, "\t\t</g>");
	print_path("M 160,300 L 185,300", "#000000", "#000000", 2);
	print_path("M 50,300 L 85,300", "none", "#000000", 2);
	print_path("M 60,300 L 60,155 L 95,155", "none", "#000000", 2);
	fprintf(f, "<path style=\"opacity:1;fill:#000000;fill-opacity:1;stroke:#000000;stroke-width:2.00000024\" d=\"M -59.154329,230.76032 L -59.154329,234.76032 L -51.154329,232.76032 L -59.154329,230.76032\" id=\"path2444\" />");
	print_use(0, 0, 136.15433,67.23968, 600, 500);
	print_use(0, 0, 151.15433, -77.76032, 600, 500);
	fprintf(f, "\t\t<g ");
	print_transform(160, -75);
	print_rect(175, 25, 350, 25, "#ffffff", "#000000", 2);
	print_path("M 25,525 L 37.5,505 L 50,525", "none", "#000000", 2);
	print_text_transform(-33.682617, 349.76276, 12, "middle", "#000000", "none", 1, 0,1,-1,0,0,0, -33.682617, 349.76276, "IF / ID");
	fprintf(f, "\t\t</g>\n");
	print_use(0, 0, 236.15433,67.23968, 600, 500);
	print_path("M 210,300 L 225,300", "#000000", "#000000", 2);
	print_path("M 260,165 L 225,165 L 225,339.99999 L 260,339.99999", "none", "#000000", 2);
	print_use(0,0, 316.48765,107.23968,600,500);
	print_use(0,0, 316.15433,-67.76032,600,500);
	print_path("M 224.98369,190 L 259.98369,190", "none", "#000000", 2);
	print_use(0,0, 316.15433,-42.76032,600,500);
	fprintf(f, "\t\t<g " );
	print_transform(325, -75);
	print_rect(175, 25, 350, 25, "#ffffff", "#000000", 2);
	print_path("M 25,525 L 37.5,505 L 50,525", "none", "#000000", 2);
	print_text_transform(-33.682617, 349.60162, 12, "middle", "#000000","none", 1, 0,1,-1,0,0,0, -33.682617,349.60162, "ID / EX");
	fprintf(f, "\t\t</g>\n" );
	print_use(0,0, 276.15433,67.23968,600,500);
	print_text(335, 320, 12, "start", "#000000", "none", 1, 335, 320, "Imm");
	print_path("M 324.57143,200 L 335,200 L 335,280 L 350,280", "none", "#000000", 2);
	print_use(0,0, 401.15433,47.23968,600,500);
	print_text(160, 230, 12, "start", "#000000", "none", 1, 160, 230, "RS1");
	print_text(185, 230, 12, "start", "#000000", "none", 1, 185, 230, "RS2");
	fprintf(f, "<path style=\"fill:#000000;fill-opacity:1;stroke:#000000;stroke-width:2\" d=\"M 235,82.5 A 2.5,2.5 0 1 1 230,82.5 A 2.5,2.5 0 1 1 235,82.5 z\" transform=\"translate(-7.500002,107.5)\" />");
	fprintf(f, "<path style=\"fill:#000000;fill-opacity:1;stroke:#000000;stroke-width:2\" d=\"M 235,82.5 A 2.5,2.5 0 1 1 230,82.5 A 2.5,2.5 0 1 1 235,82.5 z\" transform=\"translate(-172.5,217.5)\" />");
	print_path("M 315,340 L 345,340", "#000000","#000000", 2.00000024);
	print_use(0,0, 401.48765,107.23968,600,500);
	fprintf(f, "<g " );
	print_transform(-20, 70);
	print_rect(110, 440, 30, 40, c2, "#000000", 2);
	print_text(129.36816, 460.48569, 12, "middle", "#000000", "none", 1, 129.36816, 460.48569, "Zero?");
	fprintf(f, "</g>\n" );
	print_path("M 445,280 L 485,300 L 485,350 L 445,370 L 445,340 L 455,335 L 455,315 L 445,310 L 445,280 z", c2, "#000000", 2);
	print_text(329.28906, 470.33594, 12, "middle", "#000000", "none", 1, 329.28906, 470.33594, "ALU");
	fprintf(f, "<g ");
	print_transform(-59.250002,106);
	print_path("M 469.25,164 L 469.25,214 L 489.25,204 L 489.25,174 L 469.25,164 z", c2, "#000000", 2);
	print_text_transform(-475, 188.6875,12, "middle", "#000000","none", 1, 0,1,-1,0,0,0,-475, 188.6875, "MUX");
	fprintf(f, "</g>\n");
	print_path("M 430.4,295 L 445.4,295", "none", "#000000", 2);
	print_use(0,0, 496.15433,62.23968,600,500);
	fprintf(f, "<g ");
	print_transform(490,-70.00015);
	print_rect(175, 25, 350, 25, "#ffffff", "#000000", 2);
	print_path("M 25,525 L 37.5,505 L 50,525", "none", "#000000", 2);
	print_text_transform(-33.682617, 350.00006, 12, "middle", "#000000", "none", 1, 0,1,-1,0,0,0,-33.682617, 350.00006, "EX / MEM");
	fprintf(f, "</g>\n");
	print_path("M 485,325 L 510,325", "none", "#000000", 2);
	print_use(0,0, 566.15433,92.23968,600,500);
	fprintf(f, "<g ");
	print_transform(475,125);
	print_rect(175, 100, 100, 75, c3, "#000000", 2);
	print_text(228.12598, 112.67969, 12, "start", "#000000", "none", 1, 228.12598, 112.67969, "Memory");
	fprintf(f, "</g>\n");
	fprintf(f, "<g ");
	print_transform(265.75,121);
	print_path("M 469.25,164 L 469.25,214 L 489.25,204 L 489.25,174 L 469.25,164 z", c4, "#000000", 2);
	print_text_transform(-475, 188.6875,12, "middle", "#000000","none", 1, 0,1,-1,0,0,0, -475, 188.6875, "MUX");
	fprintf(f, "</g>\n");
	fprintf(f, "<g " );
	print_transform(655,-75.00015);
	print_rect(175, 25, 350, 25, "#ffffff", "#000000", 2);
	print_path("M 25,525 L 37.5,505 L 50,525", "none", "#000000", 2);
	print_text_transform(-33.682617, 350.00006, 12, "middle", "#000000", "none", 1, 0,1,-1,0,0,0,-33.682617, 350.00006, "MEM / WB");
	fprintf(f, "</g>\n" );
	fprintf(f, "<g ");
	print_transform(130.75,-39);
	print_path("M 469.25,164 L 469.25,214 L 489.25,204 L 489.25,174 L 469.25,164 z", c3, "#000000", 2);
	print_text_transform(-475, 188.6875,12, "middle", "#000000","none", 1, 0,1,-1,0,0,0,-475, 188.6875, "MUX");
	fprintf(f, "</g>\n");
	print_use(0,0, 461.15433,47.239684,600,500);
	print_path("M 375,280 L 405,280", "none", "#000000", 2);
	print_use(0,0,461.15433,77.23968,600,500);
	print_path("M 210,110 L 350,110", "none", "#000000", 2);
	print_path("M 374.9,110 L 509.9,110", "none", "#000000", 2);
	print_use(0,0,566.15433,-122.76032,600,500);
	print_path("M 405,310 L 385,310 L 385,110", "none", "#000000", 2);
	print_path("M 395,280.00001 L 395,194.97602 L 415,195", "none", "#000000", 2);
	fprintf(f, "<path style=\"opacity:1;fill:#000000;fill-opacity:1;stroke:#000000;stroke-width:2\" d=\"M 235,82.5 A 2.5,2.5 0 1 1 230,82.5 A 2.5,2.5 0 1 1 235,82.5 z\" transform=\"translate(162.5,197.5)\" />");
	fprintf(f, "<path style=\"opacity:1;fill:#000000;fill-opacity:1;stroke:#000000;stroke-width:2\" d=\"M 235,82.5 A 2.5,2.5 0 1 1 230,82.5 A 2.5,2.5 0 1 1 235,82.5 z\" transform=\"translate(152.5,27.5)\" />");
	print_use(0,0, 401.15433,-122.76032,600,500);
	fprintf(f, "<g ");
	print_transform(-59.250006,166);
	print_path("M 469.25,164 L 469.25,214 L 489.25,204 L 489.25,174 L 469.25,164 z", c2, "#000000", 2);
	print_text_transform(-475, 188.6875, 12, "middle","#000000", "none", 1, 0,1,-1,0,0,0,-475, 188.6875, "MUX");
	fprintf(f, "</g>\n");
	print_path("M 430.4,355 L 445.4,355", "none", "#000000", 2);
	print_use(0,0,496.15433,122.23968,600,500);
	print_path("M 410,370 L 375,370", "none", "#000000", 2);
	print_path("M 390,369.99999 L 390,400.04543 L 495,400 L 495,375 L 515,375", "none", "#000000", 2);
	print_path("M 375,340 L 405,340", "none", "#000000", 2);
	print_use(0,0,461.15433,107.23968,600,500);
	print_use(0,0,461.15433,137.23968,600,500);
	fprintf(f, "<path style=\"opacity:1;fill:#000000;fill-opacity:1;stroke:#000000;stroke-width:2\" d=\"M 235,82.5 A 2.5,2.5 0 1 1 230,82.5 A 2.5,2.5 0 1 1 235,82.5 z\" transform=\"translate(157.5,287.5)\" />");
	print_path("M 459.42859,195 L 514.42859,195", "none", "#000000", 2);
	print_use(0,0, 566.15433,-37.76032,600,500);
	print_use(0,0, 471.15433,-37.76032,600,500);
	print_path("M 540,325 L 570,325", "none", "#000000", 2);
	print_use(0,0, 626.15433,92.239683,600,500);
	print_path("M 540,375 L 550,375 L 570,375", "none", "#000000", 2);
	print_use(0,0, 626.15433,142.23968,600,500);
	print_path("M 600,165 L 555,165 L 555,325", "none", "#000000", 2);
	fprintf(f, "<path style=\"opacity:1;fill:#000000;fill-opacity:1;stroke:#000000;stroke-width:2\" d=\"M 235,82.5 A 2.5,2.5 0 1 1 230,82.5 A 2.5,2.5 0 1 1 235,82.5 z\" transform=\"translate(322.5,242.5)\" />");
	print_use(0,0, 731.15433,37.23968,600,500);
	print_use(0,0, 651.15433,-67.76032,600,500);
	print_path("M 540,110 L 560,110 L 560,135 L 600,135", "none", "#000000", 2);
	print_use(0,0,651.15433,-97.76032,600,500);
	print_path("M 650.44,350 L 675.24,350", "none", "#000000", 2);
	print_use(0,0,731.15433,117.23968,600,500);
	print_path("M 705,270 L 720,270 L 720,295 L 735,295", "none", "#000000", 2);
	print_path("M 735,325 L 720,325 L 720,350 L 705,350", "none", "#000000", 2);
	print_use(0,0, 786.15433,92.23968,600,500);
	print_use(0,0, 786.15433,62.23968,600,500);
	print_use(0,0, 566.15433,142.23968,600,500);
	print_path("M 540,195 L 610,195 L 610,170", "none", "#000000", 2);
	print_use(0,0, 566.15433,142.23968,600,500);
	print_path("M 540,195 L 610,195 L 610,170", "none", "#000000", 2);
	print_path("M 540,195 L 610,195 L 610,170", "none", "#000000", 2);
	print_use_transform(0,0, 0,-1,1,0,377.23968,118.84567, 600,500);
	print_path("M 755,310 L 780,310 L 780,475 L 235,475 L 235,215 L 265,215", "none", "#000000", 2);
	print_path("M 265,240 L 245,240 L 245,460 L 725,460 L 725,425 L 705,425", "none", "#000000", 2);
	print_use(0,0, 316.15433,-17.76032,600,500);
	print_use(0,0, 316.15433,7.23968,600,500);
	print_path("M 620,150 L 640,150 L 640,90 L 10,90 L 10,300 L 25,300", "none", "#000000", 2);
	print_use(0,0, 76.154329,67.23968,600,500);
	print_path("M 130,135 L 155,135 L 155,110 L 185,110", "none", "#000000", 2);
	print_use(0,0, 236.15433,-122.76032,600,500);
	print_text(123.5, 278.05078, 12, "middle", "#000000", "none", 1, 123.5, 278.05078, "Next SEQ PC");
	print_text(123.5, 443.05078, 12, "middle", "#000000", "none", 1, 123.5, 443.05078, "Next SEQ PC");
	print_text(490, 741.28516, 12, "middle", "#000000", "none", 1, 490, 741.28516, "WB Data");
	print_text2(180, 465, 12, "start", "#000000", "none", 1, 180, 465, "Branch", 192, 465, "taken");
	print_text(295, 165, 12, "start", "#000000", "none", 1, 295, 165, "IR");
	print_text(27.167969, 108.0625, 16, "middle", "#000000", "none", 1, 27.167969, 108.0625, "Instruction Fetch");
	print_text(102, 612.0625, 12, "middle", "#000000", "none", 1, 102, 612.0625, "Next PC");
	print_text2(17.617188, 280.14063, 16, "middle", "#000000", "none", 1, 17.617188, 280.14063, "Instruction Decode", 33.617188, 280.14063, "Register Fetch");
	print_text2(18.921875, 446.30469, 16, "middle", "#000000", "none", 1, 18.921875, 446.30469, "Execute", 34.921875, 446.30469, "Address Calc.");
	print_text(25, 753.46875, 16, "middle", "#000000", "none", 1, 25, 753.46875, "Write Back");
	print_text(25.371094, 610.10938, 16, "middle", "#000000", "none", 1, 25.371094, 610.10938, "Memory Access");
	print_path("M 693,5 L 693,69.999951", "none", "#000000", 2);
	print_path("M 528,5 L 528,69.999951", "none", "#000000", 2);
	print_path("M 363,5 L 363,69.999951", "none", "#000000", 2);
	print_path("M 198,5 L 198,69.999951", "none", "#000000", 2);
	print_text(65, 99.058594, 20, "start", "#000000", "none", 1, 65, 99.058594, "IF");
	print_text(65, 269.45996, 20, "start", "#000000", "none", 1, 65, 269.45996, "ID");
	print_text(65, 431.65234, 20, "start", "#000000", "none", 1, 65, 431.65234, "EX");
	print_text(65, 586.90625, 20, "start", "#000000", "none", 1, 65, 586.90625, "MEM");
	print_text(65, 737.33594, 20, "start", "#000000", "none", 1, 65, 737.33594, "WB");
	print_path("M 555,270 L 680,270", "none", "#000000", 2);
	fprintf(f, "<path style=\"fill:#000000;fill-opacity:1;stroke:#000000;stroke-width:2\" d=\"M 235,82.5 A 2.5,2.5 0 1 1 230,82.5 A 2.5,2.5 0 1 1 235,82.5 z\" transform=\"translate(322.5,187.5)\" />\n");
	print_path("M 225,340 L 225,425 L 350,425", "none", "#000000", 2);
	print_path("M 515,425 L 375,425", "#000000", "#000000", 2);
	print_path("M 680,425 L 540,425", "#000000", "#000000", 2);
	print_use(0,0, 401.15433,192.23968,600,500);
	print_use(0,0, 566.15433,192.23968,600,500);
	print_use(0,0, 731.15433,192.23968,600,500);
	fprintf(f, "<path style=\"opacity:1;fill:#000000;stroke:#000000;stroke-width:2\" d=\"M 235,82.5 A 2.5,2.5 0 1 1 230,82.5 A 2.5,2.5 0 1 1 235,82.5 z\" transform=\"translate(-7.5,257.5)\" />\n");
	fprintf(f, "\t</g>\n</svg>");
	fclose(f);
}