const float offset = 131076.f, limit = 4.f;
static const float fasterfr_lut[] = {2.86970384e-007f, 6.98202153e-008f, 3.54723927e-007f, 8.68641775e-008f, 4.50530859e-007f, 1.1115078e-007f, 5.71067776e-007f, 1.41950271e-007f, 7.22402954e-007f, 1.80930525e-007f, 9.12010443e-007f, 2.30165044e-007f, 1.14907463e-006f, 2.92225961e-007f, 1.44485566e-006f, 3.70296947e-007f, 1.81312679e-006f, 4.68309938e-007f, 2.27069621e-006f, 5.91110293e-007f, 2.83802819e-006f, 7.44655799e-007f, 3.53998029e-006f, 9.36255834e-007f, 4.40667579e-006f, 1.17485807e-006f, 5.47453348e-006f, 1.47139128e-006f, 6.78747948e-006f, 1.83917416e-006f, 8.39836935e-006f, 2.29440163e-006f, 1.0370652e-005f, 2.8567217e-006f, 1.27803109e-005f, 3.54991809e-006f, 1.57181218e-005f, 4.40271565e-006f, 1.92922711e-005f, 5.44972827e-006f, 2.36313827e-005f, 6.73257137e-006f, 2.88880062e-005f, 8.30116392e-006f, 3.5242624e-005f, 1.02152481e-005f, 4.29082409e-005f, 1.25461581e-005f, 5.21356212e-005f, 1.53788726e-005f, 6.32192468e-005f, 1.88143903e-005f, 7.65040718e-005f, 2.29724706e-005f, 9.23931533e-005f, 2.79947869e-005f, 0.000111356241f, 3.40485431e-005f, 0.000133939414f, 4.13306093e-005f, 0.000160775841f, 5.00722364e-005f, 0.00019259777f, 6.05444135e-005f, 0.00023024981f, 7.3063937e-005f, 0.000274703602f, 8.80002631e-005f, 0.000327073943f, 0.000105783219f, 0.000388636445f, 0.00012691165f, 0.000460846765f, 0.000151963088f, 0.000545361464f, 0.000181604512f, 0.00064406051f, 0.000216604297f, 0.000759071426f, 0.000257845411f, 0.000892795059f, 0.00030633996f, 0.00104793291f, 0.000363245132f, 0.00122751596f, 0.000429880614f, 0.00143493479f, 0.000507747537f, 0.00167397091f, 0.000598548981f, 0.00194882902f, 0.000704212084f, 0.00226416991f, 0.000826911738f, 0.00262514371f, 0.00096909588f, 0.00303742301f, 0.00113351232f, 0.00350723548f, 0.00132323703f, 0.00404139539f, 0.00154170385f, 0.00464733345f, 0.00179273531f, 0.00533312423f, 0.00208057462f, 0.00610751062f, 0.00240991833f, 0.00697992419f, 0.00278594963f, 0.00796050096f, 0.00321437175f, 0.00906009138f, 0.00370144117f, 0.0102902636f, 0.0042540002f, 0.0116632994f, 0.00487950837f, 0.0131921808f, 0.00558607209f, 0.0148905678f, 0.00638247189f, 0.0167727648f, 0.00727818669f, 0.0188536756f, 0.00828341422f, 0.0211487459f, 0.00940908677f, 0.0236738922f, 0.0106668816f, 0.0264454165f, 0.0120692249f, 0.0294799069f, 0.0136292888f, 0.0327941223f, 0.0153609797f, 0.0364048619f, 0.017278918f, 0.0403288201f, 0.0193984074f, 0.0445824252f, 0.0217353939f, 0.0491816643f, 0.0243064124f, 0.0541418945f, 0.0271285209f, 0.0594776408f, 0.0302192217f, 0.0652023835f, 0.0335963689f, 0.0713283366f, 0.037278061f, 0.0778662182f, 0.0412825194f, 0.084825017f, 0.0456279524f, 0.0922117572f, 0.0503324038f, 0.100031264f, 0.0554135879f, 0.108285937f, 0.0608887105f, 0.116975525f, 0.0667742761f, 0.126096925f, 0.0730858846f, 0.135643986f, 0.0798380154f, 0.145607343f, 0.0870438045f, 0.155974271f, 0.0947148124f, 0.166728569f, 0.102860788f, 0.17785048f, 0.111489431f, 0.189316648f, 0.120606151f, 0.20110011f, 0.130213833f, 0.213170343f, 0.140312607f, 0.22549335f, 0.150899629f, 0.238031792f, 0.161968872f, 0.250745183f, 0.173510935f, 0.263590123f, 0.185512871f, 0.276520587f, 0.197958043f, 0.289488269f, 0.210825997f, 0.302442964f, 0.224092376f, 0.315333009f, 0.237728859f, 0.328105752f, 0.251703142f, 0.340708066f, 0.265978954f, 0.3530869f, 0.280516117f, 0.365189843f, 0.295270642f, 0.376965717f, 0.310194881f, 0.388365179f, 0.325237707f, 0.399341322f, 0.340344754f, 0.409850276f, 0.355458692f, 0.419851788f, 0.370519546f, 0.429309781f, 0.385465056f, 0.438192881f, 0.40023108f, 0.446474901f, 0.414752024f, 0.454135269f, 0.428961312f, 0.461159407f, 0.442791874f, 0.467539032f, 0.456176665f, 0.473272396f, 0.469049196f, 0.478364437f, 0.481344078f, 0.482826852f, 0.492997568f, 0.486678086f, 0.503948118f, 0.489943228f, 0.514136911f, 0.49265382f, 0.523508392f, 0.494847583f, 0.532010762f, 0.496568052f, 0.539596464f, 0.497864136f, 0.546222617f, 0.498789594f, 0.551851425f, 0.499402452f, 0.556450536f, 0.499764355f, 0.559993349f, 0.499939859f, 0.56245927f, 0.499995698f, 0.563833915f, 0.5f, 0.564109247f, 0.500021498f, 0.563283654f, 0.500128722f, 0.561361967f, 0.500389202f, 0.55835541f, 0.500868678f, 0.554281494f, 0.501630341f, 0.549163843f, 0.502734111f, 0.54303197f, 0.504235958f, 0.535920991f, 0.506187285f, 0.527871293f, 0.508634374f, 0.518928147f, 0.511617904f, 0.509141289f, 0.515172547f, 0.498564457f, 0.519326648f, 0.487254902f, 0.52410199f, 0.475272873f, 0.529513654f, 0.462681082f, 0.535569952f, 0.449544166f, 0.542272465f, 0.435928131f, 0.549616153f, 0.421899812f, 0.557589552f, 0.407526331f, 0.566175044f, 0.392874574f, 0.575349199f, 0.378010688f, 0.585083178f, 0.362999597f, 0.595343195f, 0.347904552f, 0.606091015f, 0.33278672f, 0.617284505f, 0.317704795f, 0.628878203f, 0.302714664f, 0.640823908f, 0.287869103f, 0.653071284f, 0.273217528f, 0.665568463f, 0.258805777f, 0.678262641f, 0.24467595f, 0.691100658f, 0.23086628f, 0.70402956f, 0.21741106f, 0.716997126f, 0.204340602f, 0.729952366f, 0.19168124f, 0.742845972f, 0.179455371f, 0.755630733f, 0.167681525f, 0.768261899f, 0.15637448f, 0.780697493f, 0.145545387f, 0.792898575f, 0.135201939f, 0.804829458f, 0.125348547f, 0.816457867f, 0.115986539f, 0.827755052f, 0.107114382f, 0.83869585f, 0.0987278973f, 0.849258709f, 0.0908205015f, 0.859425657f, 0.0833834405f, 0.869182244f, 0.0764060295f, 0.878517441f, 0.0698758912f, 0.887423509f, 0.0637791893f, 0.895895844f, 0.0581008566f, 0.903932794f, 0.0528248148f, 0.911535461f, 0.0479341839f, 0.918707485f, 0.0434114806f, 0.925454819f, 0.0392388044f, 0.931785499f, 0.0353980093f, 0.937709408f, 0.0318708627f, 0.943238041f, 0.0286391884f, 0.948384274f, 0.0256849959f, 0.953162136f, 0.0229905945f, 0.957586595f, 0.0205386932f, 0.961673346f, 0.0183124866f, 0.965438621f, 0.016295727f, 0.968899002f, 0.0144727836f, 0.972071254f, 0.0128286897f, 0.974972174f, 0.0113491788f, 0.977618451f, 0.0100207098f, 0.980026544f, 0.00883048258f, 0.982212572f, 0.00776644547f, 0.984192228f, 0.00681729461f, 0.985980693f, 0.00597246667f, 0.987592579f, 0.0052221256f, 0.989041872f, 0.00455714449f, 0.990341896f, 0.00396908309f, 0.991505287f, 0.00345016201f, 0.992543969f, 0.00299323416f, 0.993469153f, 0.00259175416f, 0.99429133f, 0.00223974627f, 0.995020283f, 0.00193177146f, 0.995665097f, 0.00166289404f, 0.996234178f, 0.00142864825f, 0.996735274f, 0.00122500527f, 0.997175506f, 0.00104834085f, 0.997561389f, 0.000895403863f, 0.997898871f, 0.000763286053f, 0.998193358f, 0.00064939299f, 0.998449752f, 0.000551416519f, 0.998672481f, 0.000467308687f, 0.998865534f, 0.000395257249f, 0.999032494f, 0.000333662761f, 0.999176567f, 0.000281117279f, 0.999300616f, 0.000236384637f, 0.999407189f, 0.000198382273f, 0.999498546f, 0.000166164553f, 0.999576688f, 0.000138907537f, 0.99964338f, 0.000115895119f, 0.999700177f, 9.650646e-005f, 0.99974844f, 8.02046471e-005f, 0.999789363f, 6.65264861e-005f, 0.999823986f, 5.50733563e-005f, 0.999853217f, 4.55030398e-005f, 0.999877842f, 3.75224496e-005f, 0.999898542f, 3.08811782e-005f, 0.999915905f, 2.53657931e-005f, 0.999930437f, 2.07948091e-005f, 0.999942575f, 1.70142706e-005f, 0.999952691f, 1.38938831e-005f, 0.999961103f, 1.13236347e-005f, 0.999968084f, 9.21085544e-006f, 0.999973866f, 7.47766473e-006f, 0.999978643f, 6.05876236e-006f, 0.999982582f, 4.89952248e-006f, 0.999985823f, 3.95435356e-006f, 0.999988484f, 3.18529123e-006f, 0.999990665f, 2.56079433e-006f, 0.999992448f, 2.05471769e-006f, 0.999993903f, 1.64543802e-006f, 0.999995087f, 1.31511219e-006f, 0.999996049f, 1.0490495e-006f, 0.99999683f, 8.3518189e-007f, 0.999997461f, 6.6361785e-007f, 0.99999797f, 5.26267996e-007f, 0.999998381f, 4.16531417e-007f, 0.999998711f, 3.29033771e-007f, 0.999998976f, 2.59409031e-007f, 0.999999188f, 2.04118148e-007f, 0.999999358f, 1.60298718e-007f, 0.999999493f, 1.2564071e-007f, 0.9999996f, 9.82839552e-008f, 0.999999685f, 7.67338122e-008f, 0.99999973f, 6.55159056e-008f};
