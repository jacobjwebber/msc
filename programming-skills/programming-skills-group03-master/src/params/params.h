#ifndef params_h
#define params_h

typedef struct {
    double r;
    double a;
    double b;
    double m;
    double k;
    double l;
    double dt;
    int T;
    int T_average;
} config_params;

#endif /* params_h */

void set_params(config_params *parameters, char *file_name);
