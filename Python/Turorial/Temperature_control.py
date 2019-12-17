# Virtual intruments

# temperature_control_sample
class temperature_control(tc3)
    def __init__(self, visa_addr ='ASRL1' )
        tc3.__init__(self, visa_addr )
    
    def set_sample_T(self,T)
        set(self.sp,T)
    
    def get_sample_T(self)
        All_T = get(self.fetch)
        return All_T[6]
        
    def stabilization_loop(self, T_target)
        T = get_sample_T();
        
        # Wait for the temperature to first reach T_target
        while( T < T_target):
            wait(10) ;
            T = get_sample_T() ;
        # Then wait 20 minutes
        wait(1200);
        # Temperature should now be stable
        
    def temperature_sweep(self, T)
        if ( (T<=0.02) and (T>0.008) ) : raise ValueError(' temperature_sweep is only meant to be used 8 mK and 200 mK.') ; 
        for Temp in T:
            # Setting heat power
            if Temp<=0.02:
                    set(self.heater_range,0.001)	
            if Temp>0.02 and Temp<=0.15:
                    set(self.heater_range,0.00316)
            elif Temp>0.15:
                    set(self.heater_range,0.01)
            # Seting temperature
            set_sample_T(Temp)
            # Wait for temperature to settle
            stabilization_loop();
            
    def sweep_and_measure(self, T)
        sweep_thread = threading.Thread(self.temperature_sweep, args = (T,) ) ;
        
        time_step = 10 ;# sec
        
        temps = [];
        sweep_thread.start();
        while (sweep_thread.is_alive()):
            temps.append( self.get_sample_T() )
            wait(time_step)
        temps = array(temps); # convert to numpy array
        
        height = 8;
        fig = figure()
        fig.set_size_inches( 1.62*height , height )
        plot( arange(temps.size)*time_step, temps )
        