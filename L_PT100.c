#include "L_PT100.h"
# define TYPICAL_IPT100_AT_DAC0_IS_10 1532 // uA
# define TYPICAL_IPT100_AT_DAC0_IS_7 1103 // uA
# define TYPICAL_IPT100_AT_DAC0  TYPICAL_IPT100_AT_DAC0_IS_7// uA
#define DSADC_MOVING_AVERAGE_TIMES 8
dsadc_data_t *dsadc;
const int32_t ntc_lookup_table[];
float Rpt100 = 0;
float Rline = 0;
uint8_t dsadc_moving_average_times = DSADC_MOVING_AVERAGE_TIMES;
uint8_t dsadc_fetch_finish = 0;
double pt100_temperature = 0;
double pcb_temperature = 0;
uint16_t record_qty = 0;
void _convert_signal_end_value_as_uv(int32_t *value);
void _convert_differential_value_as_uv(int32_t *value, uint8_t g);
double ntc_convert(double Rntc);
double pt100_convert(double Rpt);

enum dsadc_process_t dsadc_process = DSADC_PROCESS_END;

uint16_t record_data[RECORD_DATA_SIZE] = {0};
int16_t record_data_index = RECORD_DATA_SIZE-1;

void dsadc_procedure_init(struct dsadc_struct *_dsadc)
{
    dsadc_process = DSADC_PROCESS_START;
    dsadc=_dsadc;
    dsadc->fetch_finish = 0;
    dsadc_fetch_finish = 0;
}

void dsadc_procedure(void)
{
    switch (dsadc_process)
    {
        case DSADC_PROCESS_START:
            R_PGA_DSAD_Create();
            R_PGA_DSAD_Start();
            R_AMP_Create();
            R_AMP_Set_PowerOn();
            R_DAC_Create();
            R_DAC0_Start();
            R_AMP0_Start();
            R_AMP2_Start();
            dsadc_moving_average_times = DSADC_MOVING_AVERAGE_TIMES;
            pt100_data_fetch_result_type = PT100_SUCCESS;
            dsadc_process = POWER_OFF_DSADC_FETCH;
            break;
        case POWER_OFF_DSADC_FETCH:
            if (dsadc_fetch_finish)
            {
                dsadc_fetch_finish = 0;
                if((pt100_temperature>450)||(pt100_temperature<-50)){
                    pt100_data_fetch_result_type = PT100_SENSE_ERROR;
                    pt100_temperature = -50;
                }
                if(Rline>10000){
                    pt100_data_fetch_result_type = PT100_LINE_ERROR;
                    pt100_temperature = -49.5;
                }
                L_PT100_STOP();
                dsadc_process = SAVE_DSADC_DATA;
            }
        break;
        case SAVE_DSADC_DATA:
            dsadc->pt100_temperature=pt100_temperature ;
            record_data[record_data_index]  = (((uint16_t)(pt100_temperature + 50)) * 2);
            if (record_data_index>0)
            {
                record_data_index--;
            }else{
                record_data_index = RECORD_DATA_SIZE - 1;
            }
            dsadc_process = DSADC_PROCESS_END;
        break;                
        case DSADC_PROCESS_END:
        break;
    }
}


void L_PT100_STOP(void){
                R_PGA_DSAD_Stop();
                R_DAC0_Stop();
                R_AMP0_Stop();
                R_AMP2_Stop();
                R_AMP_Set_PowerOff();


    
                AMPC = 0U; /* stop all AMP units */
                AMPPON = 0U;    /* power-off (default) */
                AMPEN = 0U;     /* disables input clock supply */

                DACEN = 1U;     /* disables input clock supply */

                PGAPON = 0U; /* power off PGA and DS A/D */
                PGAEN = 0U;  /* disable input clock supply */
                
                AFEPWS = 0U; /* power off AFE */
                AFEEN = 0U;  /* disable input clock supply */
                AFEPON = 0U;    /* power on AFE */

}





void DSADC_PROCESS_TEST(void)
{
                Vm0 += Vm0;
                Vm0 = Vm0 / 2;
                Vm3 += Vm3;
                Vm3 = Vm3 / 2;
                Vm2 += Vm2;
                Vm2 = Vm2 / 2;
                if (dsadc_moving_average_times)
                {
                    dsadc_moving_average_times--;
                    R_PGA_DSAD_Start();
                }else{
                    dsadc_fetch_finish=1;
                    _convert_differential_value_as_uv(&Vm0,16);
                    _convert_differential_value_as_uv(&Vm3,2);
                    _convert_signal_end_value_as_uv(&Vm2);
                    Rline = Vm0 / TYPICAL_IPT100_AT_DAC0;
                    Rpt100=(float)(Vm3 - Vm0 * 2) / TYPICAL_IPT100_AT_DAC0*10000;
                    pt100_temperature = pt100_convert(Rpt100);
                    pcb_temperature = ntc_convert(Vm2);
                }
}
    union  {
        int16_t whole;
        struct 
        {
                uint8_t b0,b1;
        }byte;
    } offset_pt100;
void DSADC_PROCESS(void)
{

    offset_pt100.byte.b0 = board[DSADC_TEMPERATURE_SENSOR_OFFSET];
    offset_pt100.byte.b1 = board[DSADC_TEMPERATURE_SENSOR_OFFSET + 1];

    Vm0 += Vm0;
    Vm0 = Vm0 / 2;
    Vm3 += Vm3;
    Vm3 = Vm3 / 2;
    Vm2 += Vm2;
    Vm2 = Vm2 / 2;
    if (dsadc_moving_average_times)
    {
        dsadc_moving_average_times--;
        R_PGA_DSAD_Start();
    }else{
        dsadc_fetch_finish=1;
        _convert_differential_value_as_uv(&Vm0,16);
        _convert_differential_value_as_uv(&Vm3,2);
        _convert_signal_end_value_as_uv(&Vm2);
        Rline = Vm0 / TYPICAL_IPT100_AT_DAC0;
        Rpt100=(float)(Vm3 - Vm0 * 2) / TYPICAL_IPT100_AT_DAC0*10000;
        pt100_temperature = pt100_convert(Rpt100)+offset_pt100.whole;
        pcb_temperature = ntc_convert(Vm2);
    }
}
void _convert_differential_value_as_uv(int32_t *value,uint8_t g)
{
    *value=(*value)*125/g/8192*625/100;
}
void _convert_signal_end_value_as_uv(int32_t *value)
{
    (*value)= (*value)*125/8192*625/100+(-800000+1000000);
}


double ntc_convert(double Rntc)
{
    int Rntc_index = 0;
    for (Rntc_index = 0; Rntc_index < 198;Rntc_index++)
    {
        if (ntc_lookup_table[Rntc_index]<Rntc)
        {
            Rntc =(Rntc-ntc_lookup_table[Rntc_index]) / (ntc_lookup_table[Rntc_index + 1] - ntc_lookup_table[Rntc_index])+Rntc_index-50+1;
            break;
        }
    }
    return Rntc;
}
const int32_t pt100_lookup_table[];
double pt100_convert(double Rpt)
{
    int index = 0;
    float result = 0;
    if(Rpt>1000000){
        index = 50;
    }
    for (; index < 500;index++)
    {
        if (pt100_lookup_table[index]>Rpt)
        {
            result =(pt100_lookup_table[index]-Rpt) / (pt100_lookup_table[index] - pt100_lookup_table[index-1])+index-51;
            break;
        }
    }
    return result;
}


const int32_t pt100_lookup_table[] = {
803063,
807033,
811003,
814970,
818937,
822902,
826865,
830828,
834789,
838748,
842707,
846663,
850619,
854573,
858526,
862478,
866428,
870377,
874325,
878271,
882217,
886160,
890103,
894044,
897985,
901923,
905861,
909797,
913732,
917666,
921599,
925530,
929461,
933390,
937317,
941244,
945169,
949093,
953016,
956938,
960859,
964778,
968696,
972613,
976529,
980444,
984358,
988270,
992181,
996091,
1000000,
1003908,
1007814,
1011720,
1015624,
1019527,
1023429,
1027330,
1031229,
1035128,
1039025,
1042921,
1046816,
1050710,
1054603,
1058495,
1062385,
1066274,
1070162,
1074049,
1077935,
1081820,
1085703,
1089585,
1093467,
1097347,
1101225,
1105103,
1108980,
1112855,
1116729,
1120602,
1124474,
1128345,
1132215,
1136083,
1139950,
1143817,
1147681,
1151545,
1155408,
1159270,
1163130,
1166989,
1170847,
1174704,
1178560,
1182414,
1186268,
1190120,
1193971,
1197821,
1201670,
1205518,
1209364,
1213210,
1217054,
1220897,
1224739,
1228579,
1232419,
1236257,
1240095,
1243931,
1247766,
1251600,
1255432,
1259264,
1263094,
1266923,
1270751,
1274578,
1278404,
1282228,
1286052,
1289874,
1293695,
1297515,
1301334,
1305152,
1308968,
1312783,
1316597,
1320411,
1324222,
1328033,
1331843,
1335651,
1339458,
1343264,
1347069,
1350873,
1354676,
1358477,
1362277,
1366077,
1369875,
1373671,
1377467,
1381262,
1385055,
1388847,
1392638,
1396428,
1400217,
1404005,
1407791,
1411576,
1415360,
1419143,
1422925,
1426706,
1430485,
1434264,
1438041,
1441817,
1445592,
1449366,
1453138,
1456910,
1460680,
1464449,
1468217,
1471984,
1475750,
1479514,
1483277,
1487040,
1490801,
1494561,
1498319,
1502077,
1505833,
1509589,
1513343,
1517096,
1520847,
1524598,
1528347,
1532096,
1535843,
1539589,
1543334,
1547078,
1550820,
1554562,
1558302,
1562041,
1565779,
1569516,
1573251,
1576986,
1580719,
1584451,
1588182,
1591912,
1595641,
1599368,
1603095,
1606820,
1610544,
1614267,
1617989,
1621709,
1625429,
1629147,
1632864,
1636580,
1640295,
1644009,
1647721,
1651433,
1655143,
1658852,
1662560,
1666267,
1669972,
1673677,
1677380,
1681082,
1684783,
1688483,
1692181,
1695879,
1699575,
1703271,
1706965,
1710658,
1714349,
1718040,
1721729,
1725418,
1729105,
1732791,
1736475,
1740159,
1743842,
1747523,
1751203,
1754882,
1758560,
1762237,
1765912,
1769587,
1773260,
1776932,
1780603,
1784273,
1787941,
1791609,
1795275,
1798940,
1802604,
1806267,
1809929,
1813590,
1817249,
1820907,
1824564,
1828220,
1831875,
1835529,
1839181,
1842832,
1846483,
1850132,
1853779,
1857426,
1861072,
1864716,
1868359,
1872001,
1875642,
1879282,
1882921,
1886558,
1890194,
1893830,
1897463,
1901096,
1904728,
1908359,
1911988,
1915616,
1919243,
1922869,
1926494,
1930117,
1933740,
1937361,
1940981,
1944600,
1948218,
1951835,
1955450,
1959065,
1962678,
1966290,
1969901,
1973510,
1977119,
1980726,
1984333,
1987938,
1991542,
1995145,
1998746,
2002347,
2005946,
2009544,
2013141,
2016737,
2020332,
2023925,
2027518,
2031109,
2034699,
2038288,
2041876,
2045463,
2049048,
2052632,
2056215,
2059798,
2063378,
2066958,
2070537,
2074114,
2077690,
2081265,
2084839,
2088412,
2091984,
2095554,
2099123,
2102692,
2106259,
2109824,
2113389,
2116953,
2120515,
2124076,
2127636,
2131195,
2134753,
2138310,
2141865,
2145419,
2148972,
2152524,
2156075,
2159625,
2163173,
2166721,
2170267,
2173812,
2177356,
2180899,
2184440,
2187981,
2191520,
2195058,
2198595,
2202131,
2205666,
2209199,
2212731,
2216263,
2219793,
2223322,
2226849,
2230376,
2233901,
2237426,
2240949,
2244471,
2247991,
2251511,
2255029,
2258547,
2262063,
2265578,
2269092,
2272605,
2276116,
2279627,
2283136,
2286644,
2290151,
2293657,
2297161,
2300665,
2304167,
2307668,
2311168,
2314667,
2318165,
2321661,
2325157,
2328651,
2332144,
2335636,
2339127,
2342616,
2346105,
2349592,
2353078,
2356563,
2360047,
2363530,
2367011,
2370492,
2373971,
2377449,
2380926,
2384402,
2387876,
2391350,
2394822,
2398293,
2401763,
2405232,
2408699,
2412166,
2415631,
2419096,
2422559,
2426021,
2429481,
2432941,
2436399,
2439857,
2443313,
2446768,
2450221,
2453674,
2457126,
2460576,
2464025,
2467473,
2470920,
2474366,
2477810,
2481254,
2484696,
2488137,
2491577,
2495016,
2498453,
2501890,
2505325,
2508759,
2512192,
2515624,
2519055,
2522485,
2525913,
2529340,
2532766,
2536191,
2539615,
2543038,
2546459,
2549879,
2553299,
2556717,
2560133,
2563549,
2566964,
2570377,
2573789,
2577200,
2580610,
2584019,
2587427,
2590833,
2594238,
2597643,
2601045,
2604447,
2607848,
2611248,
2614646,
2618043,
2621439,
2624834,
2628228,
2631620,
2635012,
2638402,
2641791,
};

const int32_t ntc_lookup_table[] = {1796652
,1796377
,1796083
,1795768
,1795431
,1795070
,1794684
,1794272
,1793832
,1793362
,1792862
,1792328
,1791760
,1791156
,1790513
,1789830
,1789104
,1788333
,1787515
,1786648
,1785729
,1784755
,1783725
,1782634
,1781481
,1780263
,1778976
,1777617
,1776184
,1774673
,1773080
,1771403
,1769637
,1767779
,1765826
,1763773
,1761616
,1759352
,1756977
,1754487
,1751876
,1749142
,1746280
,1743284
,1740152
,1736878
,1733458
,1729888
,1726163
,1722278
,1718231
,1714012
,1709622
,1705054
,1700305
,1695368
,1690240
,1684915
,1679392
,1673668
,1667738
,1661592
,1655233
,1648660
,1641863
,1634834
,1627586
,1620101
,1612390
,1604441
,1596250
,1587830
,1579158
,1570262
,1561115
,1551724
,1542097
,1532218
,1522115
,1511741
,1501152
,1490323
,1479252
,1467973
,1456448
,1444708
,1432700
,1420503
,1408110
,1395506
,1382669
,1369700
,1356514
,1343075
,1329566
,1315804
,1301902
,1287909
,1273684
,1259358
,1244873
,1230267
,1215584
,1200874
,1185928
,1170904
,1155849
,1140810
,1125685
,1110510
,1095154
,1079820
,1064556
,1049414
,1034043
,1018665
,1003540
,988275
,973127
,957895
,942857
,927801
,912754
,898027
,883095
,868564
,853876
,839360
,825051
,810649
,796516
,782692
,768851
,755007
,741566
,728173
,714846
,701602
,688889
,675878
,663457
,650758
,638710
,626406
,614327
,602495
,590932
,579661
,568178
,557550
,546736
,535733
,525100
,514859
,504453
,494470
,484932
,475253
,466049
,456716
,447252
,438298
,429876
,420690
,412717
,403975
,395807
,388235
,380582
,372844
,365022
,357837
,350579
,343247
,336585
,329862
,323077
,316229
,310088
,303896
,297653
,292147
,285804
,280211
,274576
,268900
,264000
,258244
,253276
,248276
,243243
,239024
,233931
,229662
,224508
,220187
,215842
,211473
,207960
,203548
,200000};


