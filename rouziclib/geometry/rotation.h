extern xy_t rotate_xy2(xy_t p, double th);
extern xyz_t rotate_xy(xyz_t p, double th);
extern xyz_t rotate_zxy(xyz_t p, xyz_t th);
extern xyz_t rotate_zxy_pre(xyz_t p, xyz_t costh, xyz_t sinth);
extern xyz_t rotate_x_pre(xyz_t p, double costh, double sinth);
extern xyz_t rotate_y_pre(xyz_t p, double costh, double sinth);
extern xyz_t rotate_z_pre(xyz_t p, double costh, double sinth);
extern xyz_t rotate_pt_matrix(xyz_t p, matrix_t m);
extern matrix_t rotation_matrix_zxy(xyz_t th);
