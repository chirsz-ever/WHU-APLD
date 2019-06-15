#include "discount_calc.h"
#include <QtGlobal>

double discount_calc(double amount, int vip_points)
{
    Q_ASSERT(vip_points>=0);
    if(vip_points>=150)
        return amount*0.5;
    return (1.0-vip_points/15*0.05)*amount;
}
