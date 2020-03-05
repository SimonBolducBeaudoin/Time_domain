def Window_Tukey(x,alpha,x_max) :
    # https://en.wikipedia.org/wiki/Window_function
    L = x_max + 1 ;
    X_alpha = alpha*L/2.0;
    X = (x%x_max) ;
    if 0<=X and X < X_alpha:
        return 0.5 - 0.5*cos( pi*X/X_alpha );
    elif x_max-X_alpha<=X and X < x_max :
        return 0.5 - 0.5*cos( pi*(x_max-X)/X_alpha );
    else :
        return 1;
    
def WT (x,alpha,x_max) :
    result = zeros(len(x));
    for i,X in enumerate(x) :
        result[i] = Window_Tukey(X,alpha,x_max)
    return result

def Window_Tukey_modifed(x,x_1,x_2,x_max) :
    """
    1st transition stops at x_1
    2nd transition starts at x_2
    The function's period is x_max
    """
    L = x_max + 1 ;
    X = x%x_max if sign(x)== 1 else (-x)%x_max;
    
    if sign(X)==-1:
        return Window_Tukey_modifed(-X,x_1,x_2,x_max)
    elif 0<=X and X < x_1:
        return 0.5 - 0.5*cos( pi*X/x_1 );
    elif x_2<=X and X < x_max :
        return 0.5 - 0.5*cos( pi*(x_max-X)/(x_max-x_2) );
    else :
        return 1;

def WT_mod (x,x_1,x_2,x_max) :
    result = zeros(len(x));
    for i,X in enumerate(x) :
        result[i] = Window_Tukey_modifed(X,x_1,x_2,x_max)
    return result