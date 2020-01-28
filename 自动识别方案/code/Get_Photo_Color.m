%读取图片信息并用三维图形显示出来
%distance是显示图形的像素间隔
function [hsv_classify_data,rgb_classify_data,reduce_rgb_data] = Get_Photo_Color(distance,pt_data)

%读取图片信息
data=pt_data;



%参数初始化
i_num=1;
j_num=1;
k_num=1;

%压缩图片，信息量太大不便于显示显示
for k=1:3
    for j=1:distance:size(data,2)
        for i=1:distance:size(data,1)
            reduce_rgb_data(i_num,j_num,k_num)=data(i,j,k);
            i_num=i_num+1;
        end
        j_num=j_num+1;
        i_num=1;%不要忘记了初始化
    end
    k_num=k_num+1;
    j_num=1;%不要忘记了初始化
    i_num=1;%不要忘记了初始化
end

%显示压缩的图片
% figure;
% imshow(reduce_rgb_data);

%RGB色域空间
data_rgb=double(data);
%RGB转到HSV色域空间
data_hsv=rgb2hsv(data_rgb);

%三种颜色值的加和
i_num=1;
j_num=1;
for j=1:distance:size(data,2)
    for i=1:distance:size(data,1)     
       hsv_classify_data(i_num,j_num)=data_hsv(i,j,1)+data_hsv(i,j,2)+data_hsv(i,j,3);
       rgb_classify_data(i_num,j_num)=data_rgb(i,j,1)+data_rgb(i,j,2)+data_rgb(i,j,3);
       i_num=i_num+1;
    end
   j_num=j_num+1;
   i_num=1;%不要忘记了初始化
end

x_rgb=1:distance:size(data_rgb,1);
y_rgb=1:distance:size(data_rgb,2);
x_hsv=1:distance:size(data_hsv,1);
y_hsv=1:distance:size(data_hsv,2);
x_reduce=1:1:size(hsv_classify_data,1);
y_reduce=1:1:size(hsv_classify_data,2);

%生成的为x行，y列
[X_rgb,Y_rgb]=meshgrid(y_rgb,x_rgb);
[X_hsv,Y_hsv]=meshgrid(y_hsv,x_hsv);
[X_reduce,Y_reduce]=meshgrid(y_reduce,x_reduce);

sum_data_rgb=data_rgb(:,:,1)+data_rgb(:,:,2)+data_rgb(:,:,3);
sum_data_hsv=data_hsv(:,:,1)+data_hsv(:,:,2)+data_hsv(:,:,3);

% figure;
% surf(X_rgb,Y_rgb,sum_data_rgb(x_rgb,y_rgb));
% title('处理前RGB色域空间');
% 
% figure;
% surf(X_hsv,Y_hsv,sum_data_hsv(x_hsv,y_hsv));
% title('处理前HSV色域空间');

%Find_Outline(x_redu_position,y_redu_position,hsv_classify_data);

%Draw_Outline(x_redu_position,y_redu_position,rgb_classify_data,reduce_rgb_data,200);
%Draw_line(x_redu_position,y_redu_position,reduce_rgb_data,200);
end


%找和中心点颜色相同的区域
%形参：dis_x为x轴坐标，dis_y，data存储图片数据
% function [] =Find_Outline(dis_x,dis_y,data)
%     x_max=size(data,1);
%     y_max=size(data,2);
%     book=zeros(x_max,y_max);
%     book(dis_x,dis_y)=1;
%     middle_data=data(dis_x,dis_y);
%     
%     dfs(dis_x,dis_y,data,middle_data);
%     
%     %深度优先搜索
% function [] =dfs(dis_x,dis_y,data,middle_data)
%         
%         next=[0,1;1,0;0,-1;-1,0];
% %         middle_data_sum=data(dis_x,dis_y);%初始化
%         
%         %得到一个平滑的数据，和上下左右的5个数据的平均值
% %         for i=1:4
% %              middle_data_sum=middle_data_sum+data(dis_x+next(i,1),dis_y+next(i,2));
% %          end
% %          middle_data=uint16(middle_data_sum/5);
%   
%         %5个数都变为平均数
% %         data(dis_x,dis_y)=middle_data;
% %         for i=1:4
% %             data(dis_x+next(i,1),dis_y+next(i,2))=middle_data;
% %         end
%         
%         for k=1:4
%             tx=dis_x+next(k,1);
%             ty=dis_y+next(k,2);
%             
%             if (tx<1||tx>x_max||ty<1||ty>y_max)
%                 continue;   
%             end
%             
%             if book(tx,ty)==0
%                 data(tx,ty)=middle_data;
%                 book(tx,ty)=1;
%                 dfs(tx,ty,data,middle_data);
%             end
% 
%         end
%         
%      return ;
%     
%     end
    
%end





