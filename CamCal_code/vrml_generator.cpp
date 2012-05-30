#include "stdafx.h"
#include <fstream.h>
#include "niimaq.h"
#include "parameters.h"
#include "structures.h"



int vrml_points_gen(){

	double x,y,z;

	int label;

	ofstream outdat;
	ifstream indat;

	outdat.open("points.wrl");
	indat.open("points.3D");


	outdat <<	
    "#VRML V2.0 utf8"									<< endl <<
	"# Simple x, y and z axis around 0 0 0"				<< endl <<
	"DEF coordinates Transform {"						<< endl <<
	"	children ["										<< endl <<
	"		DEF x-axis Group {"							<< endl <<
	"		  children ["								<< endl <<
	"			Transform {"							<< endl <<
	"				translation 300 0 0"				<< endl <<
	"				children ["							<< endl <<
	"				Shape {"							<< endl <<
	"					appearance Appearance {"		<< endl <<
	"					# light red"					<< endl <<
	"					material Material {"			<< endl <<
	"					 diffuseColor 1.0 0.1 0"		<< endl <<
	"					} }"							<< endl <<
	"					geometry Box {"					<< endl <<
	"						size 600 2 2"				<< endl <<
	"					} }"							<< endl <<
	"			] }"									<< endl <<
	"			Transform {"							<< endl <<
	"			   translation 600 0 0"					<< endl <<
	"			   rotation 0 0 1 -1.57"				<< endl <<
	"			   children ["							<< endl <<
	"				  Shape {"							<< endl <<
	"					 geometry Cone {"				<< endl <<
	"						bottomRadius 9"				<< endl <<
	"						height 10"					<< endl <<
	"					 }"								<< endl <<
	"					 appearance Appearance {"		<< endl <<
	"						material Material {"		<< endl <<
	"						   diffuseColor 1 0.0 0"	<< endl <<
	"						} }"						<< endl <<
	"				  }"								<< endl <<
	"				 ] }"								<< endl <<
	"		   ] }"										<< endl <<
	"	  DEF y-axis Group {"							<< endl <<
	"		 children ["								<< endl <<
	"			Transform {"							<< endl <<
	"				translation 0 300 0"				<< endl <<
	"				children ["							<< endl <<
	"			Shape {	"								<< endl <<
	"			   appearance Appearance {"				<< endl <<
	"				  # light green"					<< endl <<
	"				  material Material {"				<< endl <<
	"					 diffuseColor 0 1.0 0"			<< endl <<
	"				  } }"								<< endl <<
	"			   geometry Box {"						<< endl <<
	"				  size 2 600 2"						<< endl <<
	"			   } }"									<< endl <<
	"			] }"									<< endl <<
	"			Transform {"							<< endl <<
	"			   translation 0 600 0"					<< endl <<
	"			   children ["							<< endl <<
	"				  Shape {"							<< endl <<
	"					 geometry Cone {"				<< endl <<
	"						bottomRadius 9"				<< endl <<
	"						height 10"					<< endl <<
	"					 }"								<< endl <<
	"					 appearance Appearance {"		<< endl <<
	"						material Material {"		<< endl <<
	"						   diffuseColor 0.0 1 0.0"	<< endl <<
	"						} }"						<< endl <<
	"				  }"								<< endl <<
	"				 ] }"								<< endl <<
	"		   ] }"										<< endl <<     
	"	  DEF z-axis  Group {"							<< endl <<
	"		 children ["								<< endl <<
	"			Transform {"							<< endl <<
	"				translation 0 0 300"				<< endl <<
	"				children ["							<< endl <<
	"			Shape {"								<< endl <<
	"			   appearance Appearance {"				<< endl <<
	"				  # light blue"						<< endl <<
	"				  material Material {"				<< endl <<
	"					 diffuseColor 0 0 1.0"			<< endl <<
	"				  } }"								<< endl <<
	"			   geometry Box {"						<< endl <<
	"				  size 2 2 600"						<< endl <<
	"			   } }"									<< endl <<
	"			] }"									<< endl <<
	"			Transform {"							<< endl <<
	"			   translation 0 0 600"					<< endl <<
	"			   rotation 1 0 0 1.57"					<< endl <<
	"			   children ["							<< endl <<
	"				  Shape {"							<< endl <<
	"					 geometry Cone {"				<< endl <<
	"						bottomRadius 9"				<< endl <<
	"						height 10"					<< endl <<
	"					 }"								<< endl <<
	"					 appearance Appearance {"		<< endl <<
	"						material Material {"		<< endl <<
	"						   diffuseColor 0.0 0.0 1"	<< endl <<
	"						} }"						<< endl <<
	"				  }"								<< endl <<
	"				 ] }"								<< endl <<
	"		   ] }"										<< endl <<
	"	 ]"												<< endl <<
	"	 }"												<< endl;

	
	outdat												<<
	"Group {"											<< endl <<
	"     children ["									<< endl <<
	"	 	Shape {"									<< endl <<
	"	 	    geometry IndexedFaceSet {"				<< endl <<
	"	 		    coord Coordinate {"					<< endl <<
	"	 			    point [ 0      0    0,"			<< endl <<
	"	 						1500   0    0,"			<< endl <<
	"	 						1500   1000 0,"			<< endl <<
	"	 						0      1000 0,"			<< endl << 
	"	 						0      0	0]"			<< endl << 
	"	 			}"										<< endl << 
	"	 		coordIndex [0,1,2,3, 4, -1]"				<< endl <<
	"	 		solid	FALSE"								<< endl <<
	"	 		}#geometry"									<< endl <<
	"	 	    appearance Appearance {"					<< endl <<
	"				material Material {"					<< endl <<
	"							diffuseColor  0.1 0.8 0.25"	<< endl <<
	"							emissiveColor 0.1 0.8 0.25"	<< endl <<
	"							shininess	0.168182"		<< endl <<
	"							transparency	0.9"		<< endl <<
	"	 		    }"										<< endl <<
	"	 		}"											<< endl <<
	"	 	}#shape"										<< endl <<
	"	]"													<< endl <<
	" }"													<< endl ;

	 indat.width(4);
	 indat >> label;

	 while(!indat.eof()){
		indat.width(10);
		indat >> x;

		indat.width(10);
		indat >> y;

		indat.width(10);
		indat >> z;

		outdat <<	
		"Transform {"							<< endl <<
		"	translation  "<<x<<" "<<y<<"  "<<z  << endl <<
		"	children ["							<< endl <<
		"		Shape {"						<< endl <<
		"			appearance Appearance {"	<< endl <<
		"				material Material { }"	<< endl <<
		"			}"							<< endl <<
		"			geometry Sphere {" 			<< endl <<
		"				radius 5.0"				<< endl <<
		"			}"							<< endl <<
		"		}"								<< endl <<
		"	]"									<< endl <<
		"}"										<< endl;		
	 }
	
	indat.close();
    outdat.close();

	return  1;

}