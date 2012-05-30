#ifndef _CALIBRATION_H
#define _CALIBRATION_H

	void fcn_cal(int , int , double[], double[]);
	void fcn_cal_r(int , int , double[], double[]);
	int  Calibrate(CDC *);           
	int  Sort_points(int, int, int, char *, int);

#endif /* _CALIBRATION_H */