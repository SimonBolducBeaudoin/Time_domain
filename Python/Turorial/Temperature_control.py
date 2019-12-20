# Virtual intruments

# temperature_control_sample
class lakeshore_370_logical(instruments.lakeshore_370):
    """ 
        lakeshore_370_logical is a logical device derived from lakeshore_370
        
        If temperatures out of the intended range are used it defaults to 
        cooldown (i.e. self.heater_range is set to 0).
    """
    def __init__(self, visa_addr ='ASRL1' ):
        super(lakeshore_370_logical, self).__init__(visa_addr)
        self.current_ch.set(6); # The 6th channel is the temperature of the sample
        print " \n Current channel is : {} \n and has temperature = {} K".format( self.current_ch.get(), self.get_T() )
         
        # These tables could be fine tuned
        temperature_range_table =   [ 0.007 , 0.020     , 0.050 , 0.20      ]; # The upperbound of the interval
        temperature_tolerance =     [ 0.0   , 0.001     , 0.001 , 0.001     ]; # The upperbound of temperature error after stabilization_loop
        time_tolerance =            [ 0.0   , 60        , 60    , 60        ]; # The amount of time the temperature error as to be below the temperature_tolerance to say the stabvilization is acheived
        heater_range_table =        [ 0.0   , 0.000316  , 0.001 , 0.00316   ]; # see instruments.lakeshore_370.heater_range for all possible values
        # PID For each range ??
    
    def get_table_index(T):
        t_table = temperature_range_table;
        index = 0;
        if ( ( T<=t_table[-1] ) and ( T>t_table[0] ) ) != True : print "Temperature outside of heater_range_table" ;  return index ; 
        for i , t in enumerate(t_table):
            if ( (T <= t) == True) :
                index = i ; 
                break ;
        return index ;
        
    def set_T(self,T):
        self.set_heater_range(T)
        set(self.sp,T)
        stabilization_loop(T)
        
    def get_T(self):
        # Returns current channel temperature
        return get(self.t)
    
    def stabilization_loop(self, T_target):
        """ 
        Stabilization is acheived when we have been in a 1 mK neighborhood 
        of the target temperature for a 60 sec.
        """
        t_tol = temperature_tolerance[get_table_index(T_target)];
        time_tol = time_tolerance[get_table_index(T_target)];
        T = self.get_T();
        delta_T = abs(T - T_target);
        while( delta_T>=t_tol and ((t_1-t_0)<time_tol) ):
            t_0 = time.time(); t_1 = t_0;
            while( (delta_T<=t_tol) and ((t_1-t_0)<time_tol) ):
                wait(5)
                T = self.get_T() ;
                delta_T = abs(T - T_target);
                t_1 = time.time();
            wait(10)
            T = self.get_T() ;
            delta_T = abs(T - T_target);
    
    def set_heater_range(T):
        index = get_table_index(T) ;
        set(self.heater_range,heater_range_table[index]) ;        
        
    def _create_devs(self):
        # This could lead to looped referenced in pyHegel, but works for lakeshore_370.
        # load all existing devices
        self.current_ch_temperature = instruments.FunctionWrap( setfunc = self.set_T , getfunc = self.get_T)
        super(lakeshore_370_logical, self)._create_devs()
        # Write new devices

