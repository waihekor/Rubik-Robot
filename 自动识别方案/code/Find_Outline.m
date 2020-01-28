%compres_ratio为图片压缩比例
function [data,sum_num,middle_data,block,sort_data] = Find_Outline(compres_ratio,pt_data,mid_data)

%取整数
mid_data=round(mid_data);

[hsv_classify_data,rgb_classify_data,reduce_rgb_data] = Get_Photo_Color(compres_ratio,pt_data);

start_xy=mid_data;

data=hsv_classify_data;

x_num=size(data,1);
y_num=size(data,2);

x=1:x_num;
y=1:y_num;
[X,Y]=meshgrid(y,x);

data_buf=data(mid_data(1,1),mid_data(1,2));
data(mid_data(1,1),mid_data(1,2))=2000;

%显示处理前的图片
figure;
surf(X,Y,data(x,y));
title('处理前图片');
data(mid_data(1,1),mid_data(1,2))=data_buf;


%利用广度优先搜索寻找中心颜色块轮廓数据
[deal_data,block,sum_num(5),middle_data(5,:)]=Find_Outline_Scope(data,start_xy);

%点集按照轮廓线的顺序重新排序，以方便后续处理
sort_data=Block_Sort(block,sum_num(5));
 
%利用kmeans算法寻找中心坐标点周围的四个特定点
four_data=Find_Four_Side(sort_data,sum_num(5),middle_data(5,:));

%初步计算得到一个面中5个颜色块的中心坐标位置,2,6,4,8
[middle_data]=Compute_Five_Center(middle_data(5,:),four_data);

%
% Display_Deal_Photo(data,center_data,1000);
% title('由中心点确定其他的四个点');

%精确求得2,4,6,8
for i=2:2:8
   [deal_data,block_data,sum_num(i),middle_data(i,:)]=Find_Outline_Scope(deal_data,middle_data(i,:)); 
end

middle_data(1,:)=middle_data(2,:)+middle_data(4,:)-middle_data(5,:);
middle_data(3,:)=middle_data(2,:)+middle_data(6,:)-middle_data(5,:);
middle_data(7,:)=middle_data(4,:)+middle_data(8,:)-middle_data(5,:);
middle_data(9,:)=middle_data(6,:)+middle_data(8,:)-middle_data(5,:);

%精确求得1,3,7,9
for i=1:2:3
   [deal_data,block_data,sum_num(i),middle_data(i,:)]=Find_Outline_Scope(deal_data,middle_data(i,:)); 
end
for i=7:2:9
   [deal_data,block_data,sum_num(i),middle_data(i,:)]=Find_Outline_Scope(deal_data,middle_data(i,:)); 
end

figure;
surf(X,Y,deal_data(x,y));
title('处理后图片');

end

%利用广度优先搜索找到中心颜色块的轮廓和中心点坐标
%data存放图片信息；start_position搜索的起始位置
%data_deal存放处理后的图片信息；block存放轮廓数据；sum_num是轮廓点数目；middle_data代表中心点坐标
function [data_deal,block,sum_num,middle_data] = Find_Outline_Scope(data,start_position)

%取整数
start_position=round(start_position);

%上下左右四个方向
next=[0,1;1,0;0,-1;-1,0];

data_deal=data;

x_max=size(data_deal,1);
y_max=size(data_deal,2);

%标记检索过的数据位置
book=zeros(x_max,y_max);
 
%第一列存x坐标，第二列存y坐标
que=zeros(100000,2);%占用的内存较大，用这种方法来表示队列

%轮廓点的数目
sum_num=0;

%存放中心颜色块轮廓坐标点，第一列为x坐标，第二列为y坐标
block=zeros(2*(x_max+y_max),2);

head=1;
tail=1;
que(tail,1)=start_position(1,1);
que(tail,2)=start_position(1,2);
tail=tail+1;
book(start_position(1,1),start_position(1,2))=1;

    while head<tail
        for k=1:4
            tx=que(head,1)+next(k,1);
            ty=que(head,2)+next(k,2);
            
            if(tx<1||tx>x_max||ty<1||ty>y_max)
                continue;    
            end
            
            if data_deal(tx,ty)<(data_deal(start_position(1,1),start_position(1,2))/2)             
                sum_num=sum_num+1; 
                block(sum_num,1)=tx;
                block(sum_num,2)=ty;
                data_deal(tx,ty)=600; 
                book(tx,ty)=1;
            end        
            if (data_deal(tx,ty)>(data_deal(start_position(1,1),start_position(1,2))/2)&&book(tx,ty)==0)                 
                book(tx,ty)=1;
                que(tail,1)=tx;
                que(tail,2)=ty;
                tail=tail+1;
            end   
        end
        head=head+1;
    end
    
    %计算轮廓中心点坐标
    sum_x=sum(block(:,1));
    sum_y=sum(block(:,2));
    middle_data(1,1)=round((sum_x/sum_num));
    middle_data(1,2)=round((sum_y/sum_num));
   data_deal(middle_data(1,1),middle_data(1,2))=1000;   
end

%该函数把点集合按照轮廓线的顺序重新排序，以方便后续处理
%block_data为两列数据，第一列存放x轴数据，第二列存放y轴数据；block_data存放排好序的block_data的数据,data_num为有效数据的数目
function [sort_data]=Block_Sort(block_data,data_num)

%行数
line_num=data_num;
%
book=zeros(line_num,1);

%初始化，存放排好序的block_data的数据,第三列是序号
sort_data(1,1)=block_data(1,1);
sort_data(1,2)=block_data(1,2);
%sort_data(1,3)=1;
%标识当前sort_data中装的数据的数目
sort_datanum=1;
book(sort_datanum,1)=1;

min_dis=10000;

while sort_datanum<line_num %最后一个数据标号line_num-1
    
    for num=2:line_num
        if book(num,1)==1   %比较过了            
            continue;
        else
           dis_xy=(sort_data(sort_datanum,1)-block_data(num,1))^2+(sort_data(sort_datanum,2)-block_data(num,2))^2; 
            if min_dis>dis_xy 
                 min_dis=dis_xy;
                 min_sig=num;          
            end
            
        end

    end
    
  sort_datanum=sort_datanum+1;     
  sort_data(sort_datanum,1)=block_data(min_sig,1);
  sort_data(sort_datanum,2)=block_data(min_sig,2);  
 %sort_data(sort_datanum,3)=min_sig; 
  book(min_sig,1)=1;
  
  min_dis=10000;
  
end

end

%按照轮廓点集到中心点的距离的大小，从大到小排序
%midlle_x是中心点的x坐标，midlle_y是中心点的y坐标
%data_sort是排序后的数据
% function [data_sort]=Block_Sort_MaxMin(block_data,data_num,middle_x,middle_y)
%     data=zeros(data_num,1);
%     data_sort=zeros(data_num,1);
%     
%     for num=1:data_num
%         data(num,1)=(block_data(num,1)-middle_x)^2+(block_data(num,2)-middle_y)^2;
%     end
%     
%     %按照从大到小排序
%     [data_middle,index]=sort(data(:,1),'descend');
%    %[data_middle,index]=sort(data(:,1));
%     
%      for num=1:data_num
%         data_sort(num,1)=data(index(num));
%      end
%      
% end


%N是数据一共分多少类
%data是输入的不带分类标号的数据,u_initial是设定的每一类中心的初始值
%u是每一类的中心
%re是返回的带分类标号的数据
function [u re]=KMeans(data,N,u_initial)   
    [m n]=size(data);   %m是数据个数，n是数据维数
    
    %赋初值
    u=u_initial;
   
    while 1
        pre_u=u;            %上一次求得的中心位置
        for i=1:N
            tmp{i}=[];      % 公式一中的x(i)-uj,为公式一实现做准备
            for j=1:m
                tmp{i}=[tmp{i};data(j,:)-u(i,:)];
            end
        end
        
        quan=zeros(m,N);
        for i=1:m           %公式一的实现
            c=[];
            for j=1:N
                c=[c norm(tmp{j}(i,:))];
            end
            [junk index]=min(c);
            quan(i,index)=norm(tmp{index}(i,:));           
        end
        
        %增加的一项
        for i=1:m
            for j=1:N
                if quan(i,j)>0
                    quan(i,j)=1;  
                end
            end
        end
        
        for i=1:N %公式二的实现            
           for j=1:n
                u(i,j)=sum(quan(:,i).*data(:,j))/sum(quan(:,i));
           end           
        end
        
        if norm(pre_u-u)<1  %不断迭代直到位置不再变化
            break;
        end
    end
    
    re=[];
    for i=1:m
        tmp=[];
        for j=1:N
            tmp=[tmp norm(data(i,:)-u(j,:))];
        end
        [junk index]=min(tmp);
        re=[re;data(i,:) index];
    end
    
end


%功能：找到正方形颜色块的四个角的坐标位置
%block_data：按照点相邻的原则排序的颜色快轮廓点集；midlle_data是中心点坐标
%
function [u]=Find_Four_Side(block_data,data_num,middle_data)  

    middle_data=round(middle_data);
    data=zeros(data_num,1);
    data_sort=zeros(int16(data_num*0.5),2);
    initial_class_data=zeros(4,2);%kmens算法初始值
    
    data_dis=int16(data_num/4);
    
    for num=1:data_num
        data(num,1)=(block_data(num,1)-middle_data(1,1))^2+(block_data(num,2)-middle_data(1,2))^2;
    end
%     
%     figure;
%     plot(n,data(n,1));
    
    %从大到小的顺序排列
    %[data_middle,index]=sort(data(:,1),'descend');
    
    %从小到大的顺序排列
    [data_out,index]=sort(data(:,1));
    

     %轮廓数据按照到中心点的距离从大到小的顺序排列
     for num_data=1:int16(data_num*0.5)
        data_sort(num_data,:)=block_data(index(num_data),:);%轮廓点集数据
     end    
     
%      for nums=1:data_num
%         hold on;
%         pause(0.5);
%         plot(index(nums),data(index(nums)),'*');
%     end
     
     %计算kmens算法初始值
     data_index=index(1);
     
     for i_num=1:4
         
         if data_index>data_num
             data_index=data_index-data_num;
         end
         
         initial_class_data(i_num,1)=block_data(data_index,1); 
         initial_class_data(i_num,2)=block_data(data_index,2); 
         %initial_class_data(i_num,3)=data_index; 
         data_index=data_index+data_dis;
     end
     
%k-means聚类
[u re]=KMeans(data_sort,4,initial_class_data);  %最后产生带标号的数据，标号在所有数据的最后，意思就是数据再加一维度

u=int16(u);

[m n_data]=size(re);

%最后显示聚类后的数据
% figure;
% hold on;
% for i=1:m 
%     if re(i,3)==1   
%          plot(re(i,1),re(i,2),'ro'); 
%          plot(u(1,1),u(1,2),'k+'); 
%     elseif re(i,3)==2
%          plot(re(i,1),re(i,2),'go'); 
%          plot(u(2,1),u(2,2),'k+'); 
%     elseif re(i,3)==3 
%          plot(re(i,1),re(i,2),'bo'); 
%          plot(u(3,1),u(3,2),'k+'); 
%     else
%          plot(re(i,1),re(i,2),'co'); 
%          plot(u(4,1),u(4,2),'k+');    
%     end
% end
    
end


%由中心颜色块的中心点和其周围四个点计算其他颜色块的四个点
%data是中心块的中心点位置数据，four_data是其周围的四个点坐标
%center_data是按照一定顺序的魔方一个面的每个颜色块的中心点坐标
function [center_data]=Compute_Five_Center(data,four_data)
%先把周围四个点按照从上到下、从左到右的顺序排序(主要原理是按照点的相对位置大小)

%手动测量得到的结果
ratio_data=2.6;
%ratio_data(2)=ratio_data(1)*1.4;

sort_data=zeros(8,2);%存放中心颜色块周围的8个点数据
center_data=int16(zeros(9,2));%存放颜色块中心坐标点

%中心颜色块坐标点
center_data(5,:)=data(1,:);

[com_data,index]=sort(four_data(:,1),'descend');%y轴数据从大到小排序

%先比较y轴数据大小排在前两位的
if four_data(index(1),2)>four_data(index(2),2)%比较x轴数据的大小
    sort_data(3,:)=four_data(index(2),:);
    sort_data(4,:)=four_data(index(1),:); 
else
    sort_data(3,:)=four_data(index(1),:);
    sort_data(4,:)=four_data(index(2),:);   
end

%再比较y轴数据大小排在后两位的
if four_data(index(3),2)>four_data(index(4),2)%比较x轴数据的大小
    sort_data(1,:)=four_data(index(4),:);
    sort_data(2,:)=four_data(index(3),:); 
else
    sort_data(1,:)=four_data(index(3),:);
    sort_data(2,:)=four_data(index(4),:);   
end

sort_data(5,:)=sort_data(1,:)+sort_data(2,:)-data(1,:);
sort_data(6,:)=sort_data(1,:)+sort_data(3,:)-data(1,:);
sort_data(7,:)=sort_data(2,:)+sort_data(4,:)-data(1,:);
sort_data(8,:)=sort_data(3,:)+sort_data(4,:)-data(1,:);

%2,6,4,8
center_data(2,:)=(1-ratio_data)*data(1,:)+ratio_data*sort_data(1,:);
center_data(6,:)=(1-ratio_data)*data(1,:)+ratio_data*sort_data(2,:);
center_data(4,:)=(1-ratio_data)*data(1,:)+ratio_data*sort_data(3,:);
center_data(8,:)=(1-ratio_data)*data(1,:)+ratio_data*sort_data(4,:);

end




function []=Display_Deal_Photo(photo_data,coor_data,set_data)

coor_data=round(coor_data);

line=size(coor_data,1);%行数

for i=1:line
    photo_data(coor_data(i,1),coor_data(i,2))=set_data;
end

%显示图片
x_num=size(photo_data,1);
y_num=size(photo_data,2);

x=1:x_num;
y=1:y_num;
[X,Y]=meshgrid(y,x);

%显示处理前的图片
figure;
surf(X,Y,photo_data(x,y));
title('处理后图片');

end