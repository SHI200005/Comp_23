#include <iostream>
#include <iomanip>
#include <memory>
#include <cmath>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_odeiv2.h>
#include <rarray>
#include <netcdf>
using namespace netCDF;

// dimension of the ODEs
const unsigned zombiedim = 3;

// ODEs here
int zombierhs(double t, const double y[], double f[], void *params) {
    
    double *lparams = (double *) params;
    // get parameters from params
    double alpha = lparams[0];
    double beta = lparams[1];
    double gamma = lparams[2];
    double delta = lparams[3];
    
    f[0] = -beta*y[0]*y[2] - delta*y[0]*y[1];
    f[1] = -gamma*y[1]*y[2] + delta*y[0]*y[1];
    f[2] = beta*y[0]*y[2] + gamma*y[1]*y[2] - alpha*y[1]*y[2];
    
    return GSL_SUCCESS;
}

// netcdf_writing.cpp
void write_cdf(rmatrix<double> dataOut, std::string filename)
{
    // Create data array in memory
    int m = dataOut.extent(0); // number of rows
    int n = dataOut.extent(1); // number of columns
    
    std::cout << m<<" " << n<<"\n";

    // Create the netCDF file
    NcFile* dataFile = new NcFile(filename, NcFile::replace);
    
    // Create the dimensions
    NcDim mDim = dataFile->addDim("m", m); // dimension for the rows
    NcDim nDim = dataFile->addDim("n", n); // dimension for the columns
    
    // Create variables for each row
    std::vector<std::string> rowNames = {"t", "x", "y","z"}; // names of the rows
    std::vector<NcVar> rowVars; // vector of row variables
    for (int i = 0; i < m; i++) {
        std::string name = rowNames[i]; // name of the current row
        NcVar row = dataFile->addVar(name, ncDouble, {nDim}); // create a 1D variable for each row
        row.putVar(&dataOut[i][0]); // write the row to the variable
        rowVars.push_back(row); // add the row variable to the vector
    }
    
    // Close the file
    delete dataFile;
}


// ODEs solver
int main(int argc, char* argv[]) {
    
    // ODEs soving control
    const gsl_odeiv2_step_type* step_type = gsl_odeiv2_step_rk8pd; 
    double abstol = 0.0; // absolute error
    double reltol = 1.e-6; // relative error
    auto stepper = std::shared_ptr<gsl_odeiv2_step> (gsl_odeiv2_step_alloc(step_type, zombiedim), gsl_odeiv2_step_free);
    auto control = std::shared_ptr<gsl_odeiv2_control> (gsl_odeiv2_control_y_new (abstol, reltol), gsl_odeiv2_control_free);
    auto evolver = std::shared_ptr<gsl_odeiv2_evolve> (gsl_odeiv2_evolve_alloc(zombiedim), gsl_odeiv2_evolve_free);
    
    double param[4] = {3,2,1,1.5}; // array of the parameters!!!
    gsl_odeiv2_system sys = {zombierhs, NULL, zombiedim, param}; // system set up
    
    double y_init = std::stod(argv[1]);
    double z_init = std::stod(argv[2]);
    double x_init = 1.-y_init-z_init;
    
    double t = 0.0; // initial time
    double maxt = 200.0; // max time
    double steady_tol = 1.e-6;
    double criteria = 1.;
    double h = 1.e-3; // initial time step
    double y[zombiedim] = { x_init, y_init, z_init }; // initial condition

    // to store the time evolution
    int t_step_max = 1e3;
    rmatrix<double> y_step(t_step_max,4);
    int count = 0;
    
    // input initial condition
    y_step[0][0] = 0.;
    y_step[0][1] = y[0];
    y_step[0][2] = y[1];
    y_step[0][3] = y[2];

    // driver to solve
    while (t<maxt) {
        int status = gsl_odeiv2_evolve_apply (evolver.get(), control.get(), stepper.get(), &sys, &t, maxt, &h, y);

        if (status != GSL_SUCCESS) break;
        count = count + 1;
        
        // out put the data
        //std::cout<<std::scientific<<std::setprecision(5)<<t<<" "<<y[0]<<" "<<y[1]<<" "<<y[2]<<"\n";
        
        if (count > t_step_max){
            std::cout<<"pls redefine the maximum of time steps!\n";
            break;
        }
        
        y_step[count][0] = t;
        y_step[count][1] = y[0];
        y_step[count][2] = y[1];
        y_step[count][3] = y[2];
        
        // steady state criteria
        double alpha = param[0];
        double beta = param[1];
        double gamma = param[2];
        double delta = param[3];

        double f_0 = -beta*y[0]*y[2] - delta*y[0]*y[1];
        double f_1 = -gamma*y[1]*y[2] + delta*y[0]*y[1];
        double f_2 = beta*y[0]*y[2] + gamma*y[1]*y[2] - alpha*y[1]*y[2];
        //std::cout<<std::scientific<<std::setprecision(5)<<f_0<<" "<<f_1<<" "<<f_2<<"\n";

        criteria = sqrt(pow(f_0,2.)+pow(f_1,2.)+pow(f_2,2.))/(y[0]+y[1]+y[2]);
        //std::cout<<std::scientific<<std::setprecision(5)<<criteria<<" ";
        
        //std::cout<<std::scientific<<std::setprecision(5)<<count<<"\n";        
        
        // break the loop!
        if (criteria < steady_tol){
          break;
        }
        
    }

    // in order to output
    // Create data array in memory
    int nx = count+1; // x: t x y z
    int ny = 4;
    rmatrix<double> dataOut(ny,nx);

    for (int j=0; j<4; j++){
        for (int i=0; i<count+1; i++){
            dataOut[j][i] = y_step[i][j];
        }
    }
    
    std::string filename = argv[3];
    write_cdf(dataOut,filename);
        
    return 0;
}
