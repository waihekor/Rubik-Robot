package com.mindcont.rubikrobot.Camera;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

/**
 * 备注：SD中的存有六张图片，分别命名：1,2,3,4,5,6
 * 图片的命名和方位的关系:1:B,2:F,3:R,4:L,5:D,6:U.
 * 魔方复原算法输入的魔方颜色状态的顺序为：URFDLB,从上到下，从左到右
 */
public class ColorRecognition {

    private static final String filepath = "/storage/emulated/0/DCIM/Camera/";//SD卡路径
    private static final String filetype = ".jpg";


    /*
     *定义SD卡中图片每一面中9个中心颜色块的像素位置,照片像素：960*720
     *行对应的是颜色块的位置，每一个面有9个颜色块，按照从上到下，从左到右的顺序依次排列，
     *列对应的是x轴的位置和y轴的位置
     */


/*
    //1号
    //x轴
    private static final int color_x_position[]={527,622,728,830,927};
    //y轴

    private static final int color_y_position[]={187,283,390,486,584};
*/

//4
    //x轴
    private static final int color_x_position[]={546,636,737,828,924};
    //y轴
    private static final int color_y_position[]={207,298,385,472,560};

   /*
   //2号
    private static final int color_x_position[]={537,635,746,841,945};
    //y轴
    private static final int color_y_position[]={196,297,395,498,601};
    */

   /*
    //3号
    private static final int color_x_position[]={546,638,749,852,952};
    //y轴
    private static final int color_y_position[]={211,306,407,515,613};
    */

    //9行2列，第一列为x轴，第2列为y轴,行为9个颜色块的编号，按照从左到右，从上到下
    private static int color_block_position[][] = new int[9][2];


    private static Bitmap rawBitmap1;
    private static Bitmap rawBitmap2;
    private static Bitmap rawBitmap3;
    private static Bitmap rawBitmap4;
    private static Bitmap rawBitmap5;
    private static Bitmap rawBitmap6;


    /*
     * 调试用,储存SD卡中六张图片的中心颜色块颜色值,标号对应图片的编号
     * 行对应每张图片9个颜色块，按照从上到下，从做到右的顺序依次排列
     * 列分别对应的R、G、B的值
     */
    private static int photo1_rgb[][] = new int[9][3];
    private static int photo2_rgb[][] = new int[9][3];
    private static int photo3_rgb[][] = new int[9][3];
    private static int photo4_rgb[][] = new int[9][3];
    private static int photo5_rgb[][] = new int[9][3];
    private static int photo6_rgb[][] = new int[9][3];


    //储存颜色结果，颜色标识符为数字：1,2,3,4,5,6
    public static int photo_note[] = new int[54];
    //储存颜色结果，颜色标识符为:L,R,F,B,U,D
    public static String color_mask = new String();

    /*
     *由于进行颜色识别魔方的状态发生了改变，所以每一面对应的颜色块也要发生改变
     */
    public String AdjustColor_FirstPhoto(String surface_color) {
        String surface_variedcolor = "";
        surface_variedcolor += surface_color.charAt(6);
        surface_variedcolor += surface_color.charAt(3);
        surface_variedcolor += surface_color.charAt(0);
        surface_variedcolor += surface_color.charAt(7);
        surface_variedcolor += surface_color.charAt(4);
        surface_variedcolor += surface_color.charAt(1);
        surface_variedcolor += surface_color.charAt(8);
        surface_variedcolor += surface_color.charAt(5);
        surface_variedcolor += surface_color.charAt(2);
        return surface_variedcolor;
    }


    public String AdjustColor_SecondPhoto(String surface_color) {

        String surface_variedcolor = "";
        surface_variedcolor += surface_color.charAt(2);
        surface_variedcolor += surface_color.charAt(5);
        surface_variedcolor += surface_color.charAt(8);
        surface_variedcolor += surface_color.charAt(1);
        surface_variedcolor += surface_color.charAt(4);
        surface_variedcolor += surface_color.charAt(7);
        surface_variedcolor += surface_color.charAt(0);
        surface_variedcolor += surface_color.charAt(3);
        surface_variedcolor += surface_color.charAt(6);
        return surface_variedcolor;
    }

    public String AdjustColor_ThirdPhoto(String surface_color) {

        String surface_variedcolor = "";
        surface_variedcolor += surface_color.charAt(2);
        surface_variedcolor += surface_color.charAt(5);
        surface_variedcolor += surface_color.charAt(8);
        surface_variedcolor += surface_color.charAt(1);
        surface_variedcolor += surface_color.charAt(4);
        surface_variedcolor += surface_color.charAt(7);
        surface_variedcolor += surface_color.charAt(0);
        surface_variedcolor += surface_color.charAt(3);
        surface_variedcolor += surface_color.charAt(6);
        return surface_variedcolor;
    }

    public String AdjustColor_FourthPhoto(String surface_color) {
        String surface_variedcolor = "";
        surface_variedcolor += surface_color.charAt(2);
        surface_variedcolor += surface_color.charAt(5);
        surface_variedcolor += surface_color.charAt(8);
        surface_variedcolor += surface_color.charAt(1);
        surface_variedcolor += surface_color.charAt(4);
        surface_variedcolor += surface_color.charAt(7);
        surface_variedcolor += surface_color.charAt(0);
        surface_variedcolor += surface_color.charAt(3);
        surface_variedcolor += surface_color.charAt(6);
        return surface_variedcolor;

    }

    public String AdjustColor_FifthPhoto(String surface_color) {

        String surface_variedcolor = "";
        surface_variedcolor += surface_color.charAt(8);
        surface_variedcolor += surface_color.charAt(7);
        surface_variedcolor += surface_color.charAt(6);
        surface_variedcolor += surface_color.charAt(5);
        surface_variedcolor += surface_color.charAt(4);
        surface_variedcolor += surface_color.charAt(3);
        surface_variedcolor += surface_color.charAt(2);
        surface_variedcolor += surface_color.charAt(1);
        surface_variedcolor += surface_color.charAt(0);
        return surface_variedcolor;

    }


    /*
     *得到RGB颜色值所对应的颜色标识
     *
     *
     */
    public static String ColorRecog(int photo_color[]) {
        int color_diff[] = new int[6];
        int key;
        String color = "";

        int R = photo_color[0];
        int G = photo_color[1];
        int B = photo_color[2];
        //和第一张图片中心颜色块差值
        color_diff[0] = (Math.abs(R - photo1_rgb[4][0]) + Math.abs(G - photo1_rgb[4][1]) + Math.abs(B - photo1_rgb[4][2]));
        //和第二张图片中心颜色块差值
        color_diff[1] = (Math.abs(R - photo2_rgb[4][0]) + Math.abs(G - photo2_rgb[4][1]) + Math.abs(B - photo2_rgb[4][2]));
        //和第三张图片中心颜色块差值
        color_diff[2] = (Math.abs(R - photo3_rgb[4][0]) + Math.abs(G - photo3_rgb[4][1]) + Math.abs(B - photo3_rgb[4][2]));
        //和第四张图片中心颜色块差值
        color_diff[3] = (Math.abs(R - photo4_rgb[4][0]) + Math.abs(G - photo4_rgb[4][1]) + Math.abs(B - photo4_rgb[4][2]));
        //和第五张图片中心颜色块差值
        color_diff[4] = (Math.abs(R - photo5_rgb[4][0]) + Math.abs(G - photo5_rgb[4][1]) + Math.abs(B - photo5_rgb[4][2]));
        //和第六张图片中心颜色块差值
        color_diff[5] = (Math.abs(R - photo6_rgb[4][0]) + Math.abs(G - photo6_rgb[4][1]) + Math.abs(B - photo6_rgb[4][2]));
        key = Get_MinKey(color_diff);


        switch (key) {
            case 0: {
                color = "L";
            }
            break;
            case 1: {
                color = "R";
            }
            break;
            case 2: {
                color = "F";
            }
            break;
            case 3: {
                color = "B";
            }
            break;
            case 4: {
                color = "U";
            }
            break;
            case 5: {
                color = "D";
            }
            break;

        }

        return (color);
    }

    /*
     *获取一个数组中最小值所在的数组标号
     */
    public static int Get_MinKey(int color_diff[]) {
        int min = color_diff[0];
        int min_key = 0;
        for (int i = 1; i < color_diff.length; i++) {
            if (min > color_diff[i]) {
                min = color_diff[i];
                min_key = i;
            }
        }
        return min_key;

    }

    /*
     *获取六张图片的信息
     */
    public static void GetPhoto_fromSD() {

        //颜色块像素排列位置
        color_block_position[0][0]=color_x_position[0];
        color_block_position[0][1]=color_y_position[2];
        color_block_position[1][0]=color_x_position[1];
        color_block_position[1][1]=color_y_position[1];
        color_block_position[2][0]=color_x_position[2];
        color_block_position[2][1]=color_y_position[0];
        color_block_position[3][0]=color_x_position[1];
        color_block_position[3][1]=color_y_position[3];
        color_block_position[4][0]=color_x_position[2];
        color_block_position[4][1]=color_y_position[2];
        color_block_position[5][0]=color_x_position[3];
        color_block_position[5][1]=color_y_position[1];
        color_block_position[6][0]=color_x_position[2];
        color_block_position[6][1]=color_y_position[4];
        color_block_position[7][0]=color_x_position[3];
        color_block_position[7][1]=color_y_position[3];
        color_block_position[8][0]=color_x_position[4];
        color_block_position[8][1]=color_y_position[2];

        rawBitmap1 = BitmapFactory.decodeFile(filepath + String.valueOf(1) + filetype);
        rawBitmap2 = BitmapFactory.decodeFile(filepath + String.valueOf(2) + filetype);
        rawBitmap3 = BitmapFactory.decodeFile(filepath + String.valueOf(3) + filetype);
        rawBitmap4 = BitmapFactory.decodeFile(filepath + String.valueOf(4) + filetype);
        rawBitmap5 = BitmapFactory.decodeFile(filepath + String.valueOf(5) + filetype);
        rawBitmap6 = BitmapFactory.decodeFile(filepath + String.valueOf(6) + filetype);
    }


    /*
     *由像素点数据得到R,G,B值
     */
    public static int[] Get_RGB(int photo_color) {
        int RGB_color[] = new int[3];
        RGB_color[0] = ((photo_color & 0xff0000) >> 16);
        RGB_color[1] = ((photo_color & 0xff00) >> 8);
        RGB_color[2] = ((photo_color & 0xff));
        return RGB_color;
    }


    /*
     *输出魔方六个面的所有的颜色状态，可以直接作为魔方解算算法的输入
     *备注：魔方解算算法的魔方状态次序：U R F D L B(6,3,2,5,4,1)
     *颜色识别完之后，魔方的状态发生了变化：
     *拍摄照片的名称和魔方次序的对应关系:1:B 2:F 3:R 4:L 5:D 6:U.
     */
    public void Get_ColorMask(int pixel_x_bias, int pixel_y_bias) {
        int block_nums;
        color_mask="";//一定一定要初始化，否则后果很严重！！！！！
        String mask_color1="";
        String mask_color2="";
        String mask_color3="";
        String mask_color4="";
        String mask_color5="";
        String mask_color6="";


        Get_AllPhoto_GRB(pixel_x_bias, pixel_y_bias);

        //U(6)
        for (block_nums = 0; block_nums < 9; block_nums++)
            mask_color1 += ColorRecog(photo6_rgb[block_nums]);
        color_mask+=mask_color1;

       //R(3)
        for (block_nums = 0; block_nums < 9; block_nums++)
            mask_color2 += ColorRecog(photo3_rgb[block_nums]);
        color_mask+=AdjustColor_ThirdPhoto(mask_color2);

        //F(2)
        for (block_nums = 0; block_nums < 9; block_nums++)
            mask_color3 += ColorRecog(photo2_rgb[block_nums]);
        color_mask+=AdjustColor_SecondPhoto(mask_color3);

        //D(5)
        for (block_nums = 0; block_nums < 9; block_nums++)
            mask_color4 += ColorRecog(photo5_rgb[block_nums]);
        color_mask+=AdjustColor_FifthPhoto(mask_color4);

        //L(4)
        for (block_nums = 0; block_nums < 9; block_nums++)
            mask_color5 += ColorRecog(photo4_rgb[block_nums]);
        color_mask+=AdjustColor_FourthPhoto(mask_color5);

        //B(1)
        for (block_nums = 0; block_nums < 9; block_nums++)
            mask_color6 += ColorRecog(photo1_rgb[block_nums]);
        color_mask+=AdjustColor_FirstPhoto(mask_color6);

    }


    /*
     *判断是否发生了颜色识别的第一类错误：54个面颜色块标识并不是每9类一组
     *输入：魔方的54个颜色块的颜色标识
     */

    public static boolean Analy_Error1() {
        int i;
        int num1 = 0;
        int num2 = 0;
        int num3 = 0;
        int num4 = 0;
        int num5 = 0;
        int num6 = 0;

        for (i = 0; i < 54; i++) {
            switch (color_mask.charAt(i)) {
                case 'L': {
                    num1++;
                }
                break;

                case 'R': {
                    num2++;
                }
                break;

                case 'F': {
                    num3++;
                }
                break;

                case 'B': {
                    num4++;
                }
                break;

                case 'U': {
                    num5++;
                }
                break;
                case 'D': {
                    num6++;
                }
                break;

                default: {

                }
                break;
            }
        }

        if (num1 == 9 && num2 == 9 && num3 == 9 && num4 == 9 && num5 == 9 && num6 == 9)
            return true;
        else
            return false;

    }


    /*
     *魔方解算算法调用
     */
    public void Get_RubikTotalColor(int x_bias, int y_bias) {
        GetPhoto_fromSD();           //读取SD中的6张图片信息
        Get_ColorMask(x_bias, y_bias); //获取54块颜色块的颜色
    }


    //调试用
    public String look_color() {

        String color = new String("");
        color += "\r\n";


        for (int i = 0; i < 54; i++) {
            color += color_mask.charAt(i);
            color += " ";

            if ((i + 1) % 9 == 0)
                color += "\r\n";
        }

        return color;

    }


    //调试用

    //得到所有的RGB值
    public static void Get_AllPhoto_GRB(int pixel_x_bias, int pixel_y_bias) {


        //第一张图片
        int block_num;
        for (block_num = 0; block_num < 9; block_num++)
            photo1_rgb[block_num] = Get_RGB(rawBitmap1.getPixel(color_block_position[block_num][0] + pixel_x_bias, color_block_position[block_num][1] + pixel_y_bias));

        for (block_num = 0; block_num < 9; block_num++)
            photo2_rgb[block_num] = Get_RGB(rawBitmap2.getPixel(color_block_position[block_num][0] + pixel_x_bias, color_block_position[block_num][1] + pixel_y_bias));

        for (block_num = 0; block_num < 9; block_num++)
            photo3_rgb[block_num] = Get_RGB(rawBitmap3.getPixel(color_block_position[block_num][0] + pixel_x_bias, color_block_position[block_num][1] + pixel_y_bias));

        for (block_num = 0; block_num < 9; block_num++)
            photo4_rgb[block_num] = Get_RGB(rawBitmap4.getPixel(color_block_position[block_num][0] + pixel_x_bias, color_block_position[block_num][1] + pixel_y_bias));

        for (block_num = 0; block_num < 9; block_num++)
            photo5_rgb[block_num] = Get_RGB(rawBitmap5.getPixel(color_block_position[block_num][0] + pixel_x_bias, color_block_position[block_num][1] + pixel_y_bias));

        for (block_num = 0; block_num < 9; block_num++)
            photo6_rgb[block_num] = Get_RGB(rawBitmap6.getPixel(color_block_position[block_num][0] + pixel_x_bias, color_block_position[block_num][1] + pixel_y_bias));

    }


    //得到所有的颜色标识
    public static void Get_color_note(int pixel_x_bias, int pixel_y_bias) {

        int block_nums;
        Get_AllPhoto_GRB(pixel_x_bias, pixel_y_bias);

        for (block_nums = 0; block_nums < 9; block_nums++)
            photo_note[block_nums] = RGB_to_ColorNote(photo1_rgb[block_nums]);

        for (block_nums = 0; block_nums < 9; block_nums++)
            photo_note[9 + block_nums] = RGB_to_ColorNote(photo2_rgb[block_nums]);

        for (block_nums = 0; block_nums < 9; block_nums++)
            photo_note[9 * 2 + block_nums] = RGB_to_ColorNote(photo3_rgb[block_nums]);

        for (block_nums = 0; block_nums < 9; block_nums++)
            photo_note[9 * 3 + block_nums] = RGB_to_ColorNote(photo4_rgb[block_nums]);

        for (block_nums = 0; block_nums < 9; block_nums++)
            photo_note[9 * 4 + block_nums] = RGB_to_ColorNote(photo5_rgb[block_nums]);

        for (block_nums = 0; block_nums < 9; block_nums++)
            photo_note[9 * 5 + block_nums] = RGB_to_ColorNote(photo6_rgb[block_nums]);


    }


    //得到RGB颜色值对应的颜色标识
    public static int RGB_to_ColorNote(int photo_color[]) {
        int color_diff[] = new int[6];
        int key;

        int R = photo_color[0];
        int G = photo_color[1];
        int B = photo_color[2];

        //和第一张图片中心颜色块差值
        color_diff[0] = (Math.abs(R - photo1_rgb[4][0]) + Math.abs(G - photo1_rgb[4][1]) + Math.abs(B - photo1_rgb[4][2]));
        //和第二张图片中心颜色块差值
        color_diff[1] = (Math.abs(R - photo2_rgb[4][0]) + Math.abs(G - photo2_rgb[4][1]) + Math.abs(B - photo2_rgb[4][2]));
        //和第三张图片中心颜色块差值
        color_diff[2] = (Math.abs(R - photo3_rgb[4][0]) + Math.abs(G - photo3_rgb[4][1]) + Math.abs(B - photo3_rgb[4][2]));
        //和第四张图片中心颜色块差值
        color_diff[3] = (Math.abs(R - photo4_rgb[4][0]) + Math.abs(G - photo4_rgb[4][1]) + Math.abs(B - photo4_rgb[4][2]));
        //和第五张图片中心颜色块差值
        color_diff[4] = (Math.abs(R - photo5_rgb[4][0]) + Math.abs(G - photo5_rgb[4][1]) + Math.abs(B - photo5_rgb[4][2]));
        //和第六张图片中心颜色块差值
        color_diff[5] = (Math.abs(R - photo6_rgb[4][0]) + Math.abs(G - photo6_rgb[4][1]) + Math.abs(B - photo6_rgb[4][2]));
        key = Get_MinKey(color_diff);

        return (key + 1);
    }

    //调试用
    public String look_colo() {

        String color = new String("");
        color += "\r\n";

        for (int i = 0; i < 54; i++) {
            color += String.valueOf(photo_note[i]);
            color += " ";

            if ((i + 1) % 9 == 0)
                color += "\r\n";
        }

        return color;

    }


    public static boolean Anal_Error1() {
        int i;
        int num1 = 0;
        int num2 = 0;
        int num3 = 0;
        int num4 = 0;
        int num5 = 0;
        int num6 = 0;

        for (i = 0; i < 54; i++) {
            switch (photo_note[i]) {
                case 1: {
                    num1++;
                }
                break;

                case 2: {
                    num2++;
                }
                break;

                case 3: {
                    num3++;
                }
                break;

                case 4: {
                    num4++;
                }
                break;

                case 5: {
                    num5++;
                }
                break;
                case 6: {
                    num6++;
                }
                break;

                default: {

                }
                break;


            }

        }


        if (num1 == 9 && num2 == 9 && num3 == 9 && num4 == 9 && num5 == 9 && num6 == 9)
            return true;
        else
            return false;

    }



}
