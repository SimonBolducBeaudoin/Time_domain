## Conceptuals
- Problems
    + (16-12-19) With current calculation methods the time of claculation 
			(time_quads + hist) is of the same order (but still higher ) than 
			acquisition. Its only going to get worst when adding multiple 
			numecrical filters.
- Questions
	+ (16-12-19) What cost the most in computation time in a 
			aquasition + time_quad (single of multi-kernels) + histograms ? 
			And what could potentially be reduced ?
- Ideas ?
    + (16-12-19) Using SIMD operations to speedup multi-kernels operations 
			on the same data vector ?
    + (16-12-19) Using CUDA for even faster FFTs (FFTWs) ?
## Paperworks
- Report
    - (12-12-19) Demonstration on analytical cases
    - (16-12-19) Revive my latex thesis
        - (10-01-20) Done
    - Start theory for linear amplifier in time
        - (10-01-20) Done
    - (15-01-20) Multivariate culumant for the blackbody experiment
        - (16-01-20) opened
        - () writing theory on multivariate cumulants 
    - (16-12-19) Summery of the project ?

## Workflow
- Git stuff
    - (16-12-19) Find how to properly update my project without always having the "detached-HEAD" bug.
    - Automate git commits (and push ?) before making actual measurments ?
    - Git doesn't distinguish between Caption.txt and caption.txt (it anoying)
- Python stuff
    - Redesign the Python directory ?
    - Learn pycharm ?
    - Learn spyder ?
- Notebook 
    - Use jupiter to summurized experiements (after or during experiement ?)
        + dont use jupyter but use markdown inside code ? and combine with markdown viewer inside notepad++
- Compiling stuff
    - Find a more intuitive, more flexible solution to replace gnu-make
- C++ 
    - Explicitly write down my directory/file conventions
- gvim 
    - look it up and test
- premake
    - A language to manage the generation of makefile
- markdown
    - Is there a for of mardown that would help to organize c++ text file and python text file ?

## Python
- Tests
    - (13-12-19) Re-make a histogram benchmark and save it
        + (16-12-19) 1D done and saved under Benchmark/Results;
        + (16-12-19) 2D done and saved under Benchmark/Results;
    - (15-12-19) Benchmark the measure + quad + hist process
    - (13-01-20) moment_k_float centered_moment_k_float and cumulant_k in Python
        + Validated up to 3rd order using numpy.random.normal
- Python/Tutorial
    - (12-12-19) Guzik V measurment 
        + Done
    - (12-12-19) Calib Gain routine
        + Manual version Done
    - (12-12-19) Quadrature measurement
        + Done
    - (12-12-19) Quadrature Histograms
        + 1D Done
        + 2D Done
    - (12-12-19) Quadrature Histograms with multiple measures
        + (13-12-19) Done
    - (13-12-19) Parrallel aquisition//Traitement 
            using python threads ( Théo. 
            ~1.6 fois plus rapide)
    - (16-12-19) Temperature control
        + (13-12-19) Formation avec Ed 
    - (16-12-19) Temperature control looped And monitored in time
        +  (17-12-19) Virtual instrument "temperature_control" is ready to be tested.
        +  (20-12-19) Virtual instrument is tested and works (not all pyHegel features are working)
            +	A first calbration for the control in temperature has been done and mesure (see: Experiences)
        +  (10-01-20) Virtual instrument class called "lakeshore_370_logical" is ready
            ++ Tested for 7 to 200 mK. 
            ++ Auto stabilization
            ++ Tables : temperature_range, heater_range , PID, etc... are tested but could be calibrated furthermore

## C/C++		
- Moment and Cumulant from histogram
    - (12-12-19) Make it work
    - (13-01-20) Created SM-Math_extra and SM-Moment_cumulants
    - (13-01-20) Added moment_k and cumulant_k member function to Histogram and their Pybind11 wrapper
        + single variable histogram (DONE and validated)
- Histogram class
    - (12-12-19) properly accomodate int 8 and int16 in 1D and 2D
    - (12-12-19) abscisse function for python
        +  Done for double type 
    - (12-12-19) Add a memory_size function
    - (13-12-19) BUG : 1D histograms.sum() is not equal to the size of the input
                double as inputs
        + (16-12-19) Fixed : #pragma reduction makes copies of the reduction variable for each thread 
                            but when this variable is also a member attribute of the class and is called inside a function 
                            called by the child thread the member attribute will be called instead of the copy made for the thread. 
                            This can lead to race conditions in the code.
        + (16-01-20) This bug was supposed to be fixed but it wasn't now it should be fixed.
- time_quadratures class
    - (12-12-19) Transfert Fresnel Sine and Cos to a special function submodule
    - (12-12-19) Add a memory_size function 
    - (16-12-19) Convert time_quadrature class into python callable function and
            create SM-Blackbody which will contain a combined code 
            specialized for the blackbody experiment.
- Special function submodule
    - (12-12-19) Create python sumodule library for special functions
    - (16-12-19) Fonction de filtrage réelle et gaussienne (autre forme ?) autour 
            deux ou trois fréquences.
- Blackbody submodule
    - (16-12-19) Create SM-Blackbody from time_quadrature
    - (16-12-19) Accumuler l'histogram a mesure que les quadratures
            sont calculées.
    - (16-12-19) Pré-calculer les kernels pour plusieurs filtres numériques.
    - (16-12-19) Minimiser le coût associer à l'utilisation de plusieur kernels et filtres.
- Time_domain library
    - (12-12-19) Make the makefile



