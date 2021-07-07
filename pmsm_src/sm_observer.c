#include "includes.h"


//20K
#define Fsmopos 0.9864//1-R*T/L=1-0.2*0.00005/0.0007375
#define Gsmopos 0.0678//T/L=0.00005/0.0007375
////40K
//#define Fsmopos 0.9932//1-R*T/L=1-0.2*0.000025/0.0007375
//#define Gsmopos 0.0339//T/L=0.000025/0.0007375


#define Kslf 0.2 //
#define Kslide 300 //


float Zalpha=0;
float Zbeta=0;

void SMO_speed(void)
{
    float Ia_err=0;
    float Ib_err=0;


    Emf_get.Ualpha=Udq_to_Ualphabeta.Alpha;
    Emf_get.Ubeta=Udq_to_Ualphabeta.Beta;
    Emf_get.ialpha=Ialphabeta_to_Idq.Alpha;
    Emf_get.ibeta=Ialphabeta_to_Idq.Beta;

    /*  Sliding mode current observer   */
    Emf_get.ialpha_est = (Fsmopos*Emf_get.ialpha_est) + (Gsmopos*(Emf_get.Ualpha-Emf_get.ealpha_est-Zalpha));
    Emf_get.ibeta_est  = (Fsmopos*Emf_get.ibeta_est)  + (Gsmopos*(Emf_get.Ubeta -Emf_get.ebeta_est -Zbeta ));

    /*  Current errors  */
    Ia_err = Emf_get.ialpha_est - Emf_get.ialpha;
    Ib_err  = Emf_get.ibeta_est  - Emf_get.ibeta;

    /*  Sliding control calculator  */
    /* v.Zalpha=sign(v.IalphaError)*v.Kslide */
    if(Ia_err>0)
        Zalpha = Kslide;
    if(Ia_err<0)
        Zalpha = -Kslide;
    if(Ia_err==0)
        Zalpha = 0;

    if(Ib_err>0)
        Zbeta = Kslide;
    if(Ib_err<0)
        Zbeta = -Kslide;
    if(Ib_err==0)
        Zbeta = 0;

    /*  Sliding control filter -> back EMF calculator   */
    Emf_get.ealpha_est = Emf_get.ealpha_est + (Kslf*(Zalpha-Emf_get.ealpha_est));
    Emf_get.ebeta_est  = Emf_get.ebeta_est  + (Kslf*(Zbeta -Emf_get.ebeta_est));


    Pll_get_speed();
}
