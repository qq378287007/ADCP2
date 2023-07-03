function readADCP(Name)

%{
Name=('SLS150_ADCP_2_20211211113000.dat');
readADCP(Name);
%}
close all;clear all;clc;

fid=fopen(Name);


fseek(fid,38,'bof');
cellNum=fread(fid,1,'uint8');%层数
pingLength = 32+32+(2+cellNum*8)*2+(2+cellNum*4)+48+32+16;%帧长

fseek(fid,0,'eof');
count = ftell(fid);%文件字节数
Ping_Num =count/(32+32+2*(cellNum*8+2)+(cellNum*4+2)+96);%帧数

RTC_year = zeros(Ping_Num,1);
RTC_month=zeros(Ping_Num,1);
RTC_day=zeros(Ping_Num,1);
RTC_hour=zeros(Ping_Num,1);
RTC_min=zeros(Ping_Num,1);
RTC_sec=zeros(Ping_Num,1);

V1= zeros(Ping_Num,cellNum);
V2= zeros(Ping_Num,cellNum);
V3= zeros(Ping_Num,cellNum);
V4= zeros(Ping_Num,cellNum);

E1= zeros(Ping_Num,cellNum);
E2= zeros(Ping_Num,cellNum);
E3= zeros(Ping_Num,cellNum);
E4= zeros(Ping_Num,cellNum);

Cor1= zeros(Ping_Num,cellNum);
Cor2= zeros(Ping_Num,cellNum);
Cor3= zeros(Ping_Num,cellNum);
Cor4= zeros(Ping_Num,cellNum);

BTV1=zeros(Ping_Num,1);
BTV2=zeros(Ping_Num,1);
BTV3=zeros(Ping_Num,1);
BTV4=zeros(Ping_Num,1);

BTDepth1=zeros(Ping_Num,1);
BTDepth2=zeros(Ping_Num,1);
BTDepth3=zeros(Ping_Num,1);
BTDepth4=zeros(Ping_Num,1);

BTE1=zeros(Ping_Num,1);
BTE2=zeros(Ping_Num,1);
BTE3=zeros(Ping_Num,1);
BTE4=zeros(Ping_Num,1);

BTCor1=zeros(Ping_Num,1);
BTCor2=zeros(Ping_Num,1);
BTCor3=zeros(Ping_Num,1);
BTCor4=zeros(Ping_Num,1);

BTP1=zeros(Ping_Num,1);
BTP2=zeros(Ping_Num,1);
BTP3=zeros(Ping_Num,1);
BTP4=zeros(Ping_Num,1);

Pressure = zeros(Ping_Num,1);
Temp     = zeros(Ping_Num,1);
Roll     = zeros(Ping_Num,1);
Pitch    = zeros(Ping_Num,1);
Heading  = zeros(Ping_Num,1);


fseek(fid,0,'bof');

for ind=1:Ping_Num
    %% *******************************数据头文件************************************** %%
    Head_ID=dec2hex(fread(fid,2,'uchar')) ;                        % 数据头ID: 0x7F,0x7F
    Number_Bytes=fread(fid,1,'uint16') ;                           % 数据总长度(32+32+2*(cellNum*8+2)+(cellNum*4+2)+96)字节)
    Offset1=fread(fid,1,'uint16') ;                                % Offset1
    Offset2=fread(fid,1,'uint16') ;                                % Offset2
    Offset3=fread(fid,1,'uint16') ;                                % Offset3
    Offset4=fread(fid,1,'uint16') ;                                % Offset4
    Offset5=fread(fid,1,'uint16') ;                                % Offset5
    Offset6=fread(fid,1,'uint16') ;                                % Offset6
    Offset7=fread(fid,1,'uint16') ;                                % Offset7
    fread(fid,14,'uchar');                                              % 14个保留字节
    %% ******************************测量参数设置*************************************** %%
    Para_ID=dec2hex(fread(fid,2,'uchar')) ;                             % 数据头ID: 0x00,0x01
    WF=fread(fid,1,'uint16')/100 ;                                      % 盲区(m)
    WS=fread(fid,1,'uint16')/100 ;                                      % 层厚(m)
    WN=fread(fid,1,'uint8') ;                                           % 层数
    WP=fread(fid,1,'uint16') ;                                           % 呯数
    TP_min=fread(fid,1,'uint8') ;                                       % 呯周期（min）
    TP_sec=fread(fid,1,'uint8') ;                                       % 呯周期（Sec）

    TF_year=fread(fid,1,'uint8') ;                                      % TF时间（年）
    TF_month=fread(fid,1,'uint8') ;                                     % TF时间（月）
    TF_day=fread(fid,1,'uint8') ;                                       % TF时间（日）
    TF_hour=fread(fid,1,'uint8') ;                                      % TF时间（时）
    TF_min=fread(fid,1,'uint8') ;                                       % TF时间（分）
    TF_sec=fread(fid,1,'uint8') ;                                       % TF时间（秒）

    TE_hour=fread(fid,1,'uint8') ;                                      % TE时间（时）
    TE_min=fread(fid,1,'uint8') ;                                       % TE时间（分）
    TE_sec=fread(fid,1,'uint8') ;                                       % TE时间（秒）

    TB_hour=fread(fid,1,'uint8') ;                                      % TB时间（时）
    TB_min=fread(fid,1,'uint8') ;                                       % TB时间（分）
    TB_sec=fread(fid,1,'uint8') ;                                       % TB时间（秒）

    TC=fread(fid,1,'uint16') ;                                          % TC数目

    RTC_year(ind)=fread(fid,1,'uint8') ;                                % 记录时间（年）
    RTC_month(ind)=fread(fid,1,'uint8') ;                               % 记录时间（月）
    RTC_day(ind)=fread(fid,1,'uint8') ;                                 % 记录时间（日）
    RTC_hour(ind)=fread(fid,1,'uint8') ;                                % 记录时间（时）
    RTC_min(ind)=fread(fid,1,'uint8') ;                                 % 记录时间（分）
    RTC_sec(ind)=fread(fid,1,'uint8') ;                                 % 记录时间（秒）
    Res1=fread(fid,1,'uchar');                                          % 1个保留字节

    %% *******************************流速剖面数据************************************ %%
    Current_ID=dec2hex(fread(fid,2,'uchar')) ;                         % 数据头ID: 0x00,0x02

    for index=1:cellNum

        V1(ind,index)= fread(fid,1,'int16=>double')/1000;                    % 第index层1#波束速度(单位:mm/s)
        V2(ind,index)= fread(fid,1,'int16=>double')/1000;                    % 第index层2#波束速度(单位:mm/s)
        V3(ind,index)= fread(fid,1,'int16=>double')/1000;                    % 第index层3#波束速度(单位:mm/s)
        V4(ind,index)= fread(fid,1,'int16=>double')/1000;                    % 第index层4#波束速度(单位:mm/s)

    end
    %% *****************************回波强度剖面数据*********************************** %%
    Intensity_ID=dec2hex(fread(fid,2,'uchar')) ;                       % 数据头ID: 0x00,0x03

    for index=1:cellNum

        E1(ind,index)= fread(fid,1,'uint16');                           % 第index层1#波束回波强度
        E2(ind,index)= fread(fid,1,'uint16');                           % 第index层2#波束回波强度
        E3(ind,index)= fread(fid,1,'uint16');                           % 第index层3#波束回波强度
        E4(ind,index)= fread(fid,1,'uint16');                           % 第index层4#波束回波强度

    end

    %% *****************************相关系数剖面数据*********************************** %%
    Correlation_ID=dec2hex(fread(fid,2,'uchar')) ;                      % 数据头ID: 0x00,0x04

    for index=1:cellNum

        Cor1(ind,index)=fread(fid,1,'uint8') ;                          % 1#波束相关系数
        Cor2(ind,index)=fread(fid,1,'uint8') ;                          % 2#波束相关系数
        Cor3(ind,index)=fread(fid,1,'uint8') ;                          % 3#波束相关系数
        Cor4(ind,index)=fread(fid,1,'uint8') ;                          % 4#波束相关系数

    end

    %% *******************************底跟踪数据************************************** %%
    BottomTrack_ID=dec2hex(fread(fid,2,'uchar')) ;                      % 数据头ID: 0x00,0x05

    BTV1(ind)=fread(fid,1,'int16=>double') ;                            % 1#波束底跟踪速度(单位:mm/s)
    BTV2(ind)=fread(fid,1,'int16=>double') ;                            % 2#波束底跟踪速度(单位:mm/s)
    BTV3(ind)=fread(fid,1,'int16=>double') ;                            % 3#波束底跟踪速度(单位:mm/s)
    BTV4(ind)=fread(fid,1,'int16=>double') ;                            % 4#波束底跟踪速度(单位:mm/s)

    BTDepth1(ind)=fread(fid,1,'uint16')/100.0 ;                         % 1#波束底跟踪深度(单位：m)
    BTDepth2(ind)=fread(fid,1,'uint16')/100.0 ;                         % 2#波束底跟踪深度(单位：m)
    BTDepth3(ind)=fread(fid,1,'uint16')/100.0 ;                         % 3#波束底跟踪深度(单位：m)
    BTDepth4(ind)=fread(fid,1,'uint16')/100.0 ;                         % 4#波束底跟踪深度(单位：m)

    BTE1(ind)=fread(fid,1,'uint16');                                    % 1#波束底跟踪回波强度
    BTE2(ind)=fread(fid,1,'uint16');                                    % 2#波束底跟踪回波强度
    BTE3(ind)=fread(fid,1,'uint16');                                    % 3#波束底跟踪回波强度
    BTE4(ind)=fread(fid,1,'uint16');                                    % 4#波束底跟踪回波强度

    BTCor1(ind)=fread(fid,1,'uint8');                                   % 1#波束底跟踪回波相关系数
    BTCor2(ind)=fread(fid,1,'uint8');                                   % 2#波束底跟踪回波相关系数
    BTCor3(ind)=fread(fid,1,'uint8');                                   % 3#波束底跟踪回波相关系数
    BTCor4(ind)=fread(fid,1,'uint8');                                   % 4#波束底跟踪回波相关系数

    BTP1(ind)=fread(fid,1,'uint8');                                     % 1#波束底跟踪数据完好率
    BTP2(ind)=fread(fid,1,'uint8');                                     % 2#波束底跟踪数据完好率
    BTP3(ind)=fread(fid,1,'uint8');                                     % 3#波束底跟踪数据完好率
    BTP4(ind)=fread(fid,1,'uint8');                                     % 4#波束底跟踪数据完好率

    fread(fid,14,'uchar');                                              % 保留14个字节

    %% ***************************传感器数据（磁罗盘、温度、压力及GPS数据）****************** %%
    Sensor_ID=dec2hex(fread(fid,2,'uchar')) ;                            % 数据头ID: 0x00,0x06
    Pressure(ind)=fread(fid,1,'uint16')/10.0;                            % 压力传感器数据(单位:m)
    Temp(ind)=fread(fid,1,'int16')/100.0;                                % 温度传感器数据(单位:℃)
    Roll(ind)=fread(fid,1,'int16')/10.0;                                 % 纵摇数据(单位:°)
    Pitch(ind)=fread(fid,1,'int16')/10.0;                                % 横摇数据(单位:°)
    Heading(ind)=fread(fid,1,'int16')/100.0;                             % 航向角数据(单位:°)
    WOE(ind)=fread(fid,1,'uchar');                                       % 经度标志位
    Long_deg=fread(fid,1,'int16') ;
    Long_sec=fread(fid,2,'int16') ;
    NOS(ind)=fread(fid,1,'uchar');                                       % 纬度标志位
    Lat_deg=fread(fid,1,'int16') ;
    Lat_sec=fread(fid,2,'int16') ;
    fread(fid,6,'uchar');                                                % 保留6个字节

    %% *******************************数据尾及校验和数据****************************** %%
    Check_ID=dec2hex(fread(fid,2,'uchar')) ;                              % 数据头ID: 0x00,0x07
    Voltage(ind)=fread(fid,1,'uint16');                                    % 电压检测
    fread(fid,10,'uchar');                                                % 保留6个字节
    Check_Sum(ind)=fread(fid,1,'uint16');                                 % 数据字节和

    ind
end
fclose(fid);


vxb=(V1-V2)/2/sind(24.6);
vyb=(V3-V4)/2/sind(24.6);
vx=0.707*(vxb-vyb);
vy=0.707*(vxb+vyb);
Vall=sqrt(vx.^2+vy.^2);


Name(end-2)='m';
Name(end-1)='a';
Name(end)='t';
save(Name,'WF','WS','WN','WP',...
    'TP_min','TP_sec','TF_year','TF_month','TF_day','TF_hour','TF_min','TF_sec','TE_hour','TE_min','TE_min',...
    'TB_hour','TB_min','TB_sec','TC','RTC_year','RTC_month','RTC_day','RTC_hour','RTC_min','RTC_sec',...
    'V1','V2','V3','V4','E1','E2','E3','E4','Cor1','Cor2','Cor3','Cor4','BTV1','BTV2','BTV3','BTV4',...
    'BTDepth1','BTDepth2','BTDepth3','BTDepth4','BTE1','BTE2','BTE3','BTE4',...
    'BTCor1','BTCor2','BTCor3','BTCor4','Pressure','Temp','Roll','Pitch','Heading','Vall');




%
%

%
% cell=1;                                                                     % 参考层绘图，选值范围1-cell_Num
% figure(1);
% plot(V1(:,cell),'k');xlabel('集合数');ylabel('波束速度(mm/s)');grid on;hold on;
% plot(V2(:,cell),'b');plot(V3(:,cell),'c');plot(V4(:,cell),'r');legend('V1','V2','V3','V4');
%
% index=2;                                                                    % index集合点上所有剖面流速分布
% figure(2);
% plot(V1(index,:),'r');xlabel('剖面单元层');ylabel('波束速度(mm/s)');
% %%顺时针
%


% AveV=[];
% StdV=[];
%
% for index=1:cellNum
%     CellRef=index;
%     AveV=[AveV,mean(v(10:200,CellRef))];
%     StdV=[StdV,std(v(10:200,CellRef))];
%     E1Ave(index)=mean(E1(10:200,CellRef));
%     E2Ave(index)=mean(E2(10:200,CellRef));
%     E3Ave(index)=mean(E3(10:200,CellRef));
%     E4Ave(index)=mean(E4(10:200,CellRef));
%     figure(3)
%     plot(v(:,CellRef));
%
%     xlabel('样本点数')
%     ylabel('速度mm/s')
%     title(['第',num2str(CellRef),'层测量的速度模值'])
%     pause;
% end
% AveV=AveV';
% StdV=StdV';
%
% Distance=WF+WS*[1:WN];
%
% figure
% plot(Distance,AveV)
% xlabel('剖面距离(m)');ylabel('速度模值(mm/s)');
% grid on;
% title('各层测量的速度均值')
%
% figure
% plot(Distance,StdV)
% xlabel('剖面距离(m)');ylabel('速度方差(mm/s)');
% grid on;
% title('各层测量的速度方差')
%
% EAve=(E1Ave+E2Ave+E3Ave+E4Ave)/4;
% EAve=EAve';