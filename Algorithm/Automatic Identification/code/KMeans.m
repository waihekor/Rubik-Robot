
%N是数据一共分多少类
%data是输入的不带分类标号的数据
%u是每一类的中心
%re是返回的带分类标号的数据
function [u re]=KMeans(data,N,initial_u)   
    [m n]=size(data);   %m是数据个数，n是数据维数
%     ma=zeros(n);        %每一维最大的数
%     mi=zeros(n);        %每一维最小的数
    u=zeros(N,n);       %随机初始化，最终迭代到每一类的中心位置
%     for i=1:n
%        ma(i)=max(data(:,i));    %每一维最大的数
%        mi(i)=min(data(:,i));    %每一维最小的数
%        for j=1:N
%             u(j,i)=ma(i)+(mi(i)-ma(i))*rand();  %随机初始化，不过还是在每一维[min max]中初始化好些
%        end      
%     end
    
        %赋初值
    u=initial_u;
   
    while 1
        pre_u=u;            %上一次求得的中心位置
        for i=1:N
            tmp{i}=[];      % 公式一中的x(i)-uj,为公式一实现做准备
            for j=1:m
                tmp{i}=[tmp{i};data(j,:)-u(i,:)];
            end
        end
        
        quan=zeros(m,N);
        for i=1:m        %公式一的实现
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
        
        for i=1:N            %公式二的实现
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


% figure;
% plot(data_sort(idx==1,1),data_sort(idx==1,2),'r.','MarkerSize',12)
% hold on
% plot(data_sort(idx==2,1),data_sort(idx==2,2),'b.','MarkerSize',12)
% hold on
% plot(data_sort(idx==3,1),data_sort(idx==3,2),'g.','MarkerSize',12)
% hold on
% plot(data_sort(idx==4,1),data_sort(idx==4,2),'m.','MarkerSize',12)
% hold on
% plot(C(1,1),C(1,2),'kx','MarkerSize',15,'LineWidth',3);
% plot(C(2,1),C(2,2),'kx','MarkerSize',15,'LineWidth',3);
% plot(C(3,1),C(3,2),'kx','MarkerSize',15,'LineWidth',3);
% plot(C(4,1),C(4,2),'kx','MarkerSize',15,'LineWidth',3);
