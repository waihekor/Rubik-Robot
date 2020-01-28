distance=5;

%图片中心块的像素坐标
xy_position(1,1)=390;
xy_position(1,2)=728;

%压缩图片后所对应的像素位置
xy_redu_position(1,1)=round((xy_position(1,1)/distance));
xy_redu_position(1,2)=round((xy_position(1,2)/distance));

data=imread('1.jpg');
[data,sum_num,middle_data,block,sort_data] = Find_Outline(distance,data,xy_redu_position);

data=imread('2.jpg');
[data,sum_num,middle_data,block,sort_data] = Find_Outline(distance,data,middle_data(5,:));

data=imread('3.jpg');
[data,sum_num,middle_data,block,sort_data] = Find_Outline(distance,data,middle_data(5,:));

data=imread('4.jpg');
[data,sum_num,middle_data,block,sort_data] = Find_Outline(distance,data,middle_data(5,:));

data=imread('5.jpg');
[data,sum_num,middle_data,block,sort_data] = Find_Outline(distance,data,middle_data(5,:));

data=imread('6.jpg');
[data,sum_num,middle_data,block,sort_data] = Find_Outline(distance,data,middle_data(5,:));


