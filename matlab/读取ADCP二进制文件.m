close all;clear all;clc;

Name='SLS150_ADCP_2_20211211113000.dat';
fid=fopen(Name);

fseek(fid,38,'bof');
cellNum=fread(fid,1,'uint8');%����
pingLength = 32+32+(2+cellNum*8)*2+(2+cellNum*4)+48+32+16;%֡��

fseek(fid,0,'eof');
count = ftell(fid);%�ļ��ֽ���
Ping_Num =count/(32+32+2*(cellNum*8+2)+(cellNum*4+2)+96);%֡��

%����ͷ�ļ�
Data_ID = cell(Ping_Num,1);
Ping_byte = zeros(Ping_Num,1);

%������������
Para_ID = cell(Ping_Num,1);
RTC_year = zeros(Ping_Num,1);
RTC_month=zeros(Ping_Num,1);
RTC_day=zeros(Ping_Num,1);
RTC_hour=zeros(Ping_Num,1);
RTC_min=zeros(Ping_Num,1);
RTC_sec=zeros(Ping_Num,1);

%������������
Current_ID = cell(Ping_Num,1);
V1= zeros(Ping_Num,cellNum);
V2= zeros(Ping_Num,cellNum);
V3= zeros(Ping_Num,cellNum);
V4= zeros(Ping_Num,cellNum);

%�ز�ǿ����������
Intensity_ID = cell(Ping_Num,1);
E1= zeros(Ping_Num,cellNum);
E2= zeros(Ping_Num,cellNum);
E3= zeros(Ping_Num,cellNum);
E4= zeros(Ping_Num,cellNum);

%���ϵ����������
Correlation_ID = cell(Ping_Num,1);
Cor1= zeros(Ping_Num,cellNum);
Cor2= zeros(Ping_Num,cellNum);
Cor3= zeros(Ping_Num,cellNum);
Cor4= zeros(Ping_Num,cellNum);

%�׸�������
BottomTrack_ID = cell(Ping_Num,1);
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

%���������ݣ������̡��¶ȡ�ѹ����GPS���ݣ�
Sensor_ID = cell(Ping_Num,1);
Pressure = zeros(Ping_Num,1);
Temp     = zeros(Ping_Num,1);
Roll     = zeros(Ping_Num,1);
Pitch    = zeros(Ping_Num,1);
Heading  = zeros(Ping_Num,1);
WOE  = zeros(Ping_Num,1);
%Long_deg  = zeros(Ping_Num,1);
%Long_sec  = zeros(Ping_Num,1);
NOS  = zeros(Ping_Num,1);
%Lat_deg  = zeros(Ping_Num,1);
%Lat_sec  = zeros(Ping_Num,1);

%����β��У�������
Check_ID = cell(Ping_Num,1);
Voltage  = zeros(Ping_Num,1);
Check_Sum  = zeros(Ping_Num,1);

fseek(fid,0,'bof');
%Ping_Num = 500;
for ind=1:Ping_Num
    %% *******************************����ͷ�ļ�************************************** %%
    Data_ID{ind}=dec2hex(fread(fid,2,'uchar')) ;                   % ����ͷID: 0x7F,0x7F
    Ping_byte(ind)=fread(fid,1,'uint16') ;                         % ֡��: 32+32+(2+cellNum*8)*2+(2+cellNum*4)+48+32+16
    Offset1=fread(fid,1,'uint16') ;                                % Offset1
    Offset2=fread(fid,1,'uint16') ;                                % Offset2
    Offset3=fread(fid,1,'uint16') ;                                % Offset3
    Offset4=fread(fid,1,'uint16') ;                                % Offset4
    Offset5=fread(fid,1,'uint16') ;                                % Offset5
    Offset6=fread(fid,1,'uint16') ;                                % Offset6
    Offset7=fread(fid,1,'uint16') ;                                % Offset7
    fread(fid,14,'uchar');                                         % 14�������ֽ�
    
    %% ******************************������������*************************************** %%
    Para_ID{ind}=dec2hex(fread(fid,2,'uchar')) ;                        % ����ͷID: 0x00,0x01
    WF=fread(fid,1,'uint16')/100 ;                                      % ä��(m)
    WS=fread(fid,1,'uint16')/100 ;                                      % ���(m)
    WN=fread(fid,1,'uint8') ;                                           % ����
    WP=fread(fid,1,'uint16') ;                                          % ����
    TP_min=fread(fid,1,'uint8') ;                                       % �����ڣ�min��
    TP_sec=fread(fid,1,'uint8') ;                                       % �����ڣ�Sec��
    
    TF_year=fread(fid,1,'uint8') ;                                      % TFʱ�䣨�꣩
    TF_month=fread(fid,1,'uint8') ;                                     % TFʱ�䣨�£�
    TF_day=fread(fid,1,'uint8') ;                                       % TFʱ�䣨�գ�
    TF_hour=fread(fid,1,'uint8') ;                                      % TFʱ�䣨ʱ��
    TF_min=fread(fid,1,'uint8') ;                                       % TFʱ�䣨�֣�
    TF_sec=fread(fid,1,'uint8') ;                                       % TFʱ�䣨�룩
    
    TE_hour=fread(fid,1,'uint8') ;                                      % TEʱ�䣨ʱ��
    TE_min=fread(fid,1,'uint8') ;                                       % TEʱ�䣨�֣�
    TE_sec=fread(fid,1,'uint8') ;                                       % TEʱ�䣨�룩
    
    TB_hour=fread(fid,1,'uint8') ;                                      % TBʱ�䣨ʱ��
    TB_min=fread(fid,1,'uint8') ;                                       % TBʱ�䣨�֣�
    TB_sec=fread(fid,1,'uint8') ;                                       % TBʱ�䣨�룩
    
    TC=fread(fid,1,'uint16') ;                                          % TC��Ŀ
    
    RTC_year(ind)=fread(fid,1,'uint8') ;                                % ��¼ʱ�䣨�꣩
    RTC_month(ind)=fread(fid,1,'uint8') ;                               % ��¼ʱ�䣨�£�
    RTC_day(ind)=fread(fid,1,'uint8') ;                                 % ��¼ʱ�䣨�գ�
    RTC_hour(ind)=fread(fid,1,'uint8') ;                                % ��¼ʱ�䣨ʱ��
    RTC_min(ind)=fread(fid,1,'uint8') ;                                 % ��¼ʱ�䣨�֣�
    RTC_sec(ind)=fread(fid,1,'uint8') ;                                 % ��¼ʱ�䣨�룩
    fread(fid,1,'uchar');                                               % 1�������ֽ�
    
    %% *******************************������������************************************ %%
    Current_ID{ind}=dec2hex(fread(fid,2,'uchar')) ;                               % ����ͷID: 0x00,0x02
    for index=1:cellNum
        V1(ind,index)= fread(fid,1,'int16=>double')/1000;                    % ��index��1#�����ٶ�(��λ:mm/s)
        V2(ind,index)= fread(fid,1,'int16=>double')/1000;                    % ��index��2#�����ٶ�(��λ:mm/s)
        V3(ind,index)= fread(fid,1,'int16=>double')/1000;                    % ��index��3#�����ٶ�(��λ:mm/s)
        V4(ind,index)= fread(fid,1,'int16=>double')/1000;                    % ��index��4#�����ٶ�(��λ:mm/s)
    end
    
    %% *****************************�ز�ǿ����������*********************************** %%
    Intensity_ID{ind}=dec2hex(fread(fid,2,'uchar')) ;                        % ����ͷID: 0x00,0x03
    for index=1:cellNum
        E1(ind,index)= fread(fid,1,'uint16');                           % ��index��1#�����ز�ǿ��
        E2(ind,index)= fread(fid,1,'uint16');                           % ��index��2#�����ز�ǿ��
        E3(ind,index)= fread(fid,1,'uint16');                           % ��index��3#�����ز�ǿ��
        E4(ind,index)= fread(fid,1,'uint16');                           % ��index��4#�����ز�ǿ��
    end
    
    %% *****************************���ϵ����������*********************************** %%
    Correlation_ID{ind}=dec2hex(fread(fid,2,'uchar')) ;                      % ����ͷID: 0x00,0x04
    for index=1:cellNum
        Cor1(ind,index)=fread(fid,1,'uint8') ;                          % 1#�������ϵ��
        Cor2(ind,index)=fread(fid,1,'uint8') ;                          % 2#�������ϵ��
        Cor3(ind,index)=fread(fid,1,'uint8') ;                          % 3#�������ϵ��
        Cor4(ind,index)=fread(fid,1,'uint8') ;                          % 4#�������ϵ��
    end
    
    %% *******************************�׸�������************************************** %%
    BottomTrack_ID{ind}=dec2hex(fread(fid,2,'uchar')) ;                      % ����ͷID: 0x00,0x05
    
    BTV1(ind)=fread(fid,1,'int16=>double') ;                            % 1#�����׸����ٶ�(��λ:mm/s)
    BTV2(ind)=fread(fid,1,'int16=>double') ;                            % 2#�����׸����ٶ�(��λ:mm/s)
    BTV3(ind)=fread(fid,1,'int16=>double') ;                            % 3#�����׸����ٶ�(��λ:mm/s)
    BTV4(ind)=fread(fid,1,'int16=>double') ;                            % 4#�����׸����ٶ�(��λ:mm/s)
    
    BTDepth1(ind)=fread(fid,1,'uint16')/100.0 ;                         % 1#�����׸������(��λ��m)
    BTDepth2(ind)=fread(fid,1,'uint16')/100.0 ;                         % 2#�����׸������(��λ��m)
    BTDepth3(ind)=fread(fid,1,'uint16')/100.0 ;                         % 3#�����׸������(��λ��m)
    BTDepth4(ind)=fread(fid,1,'uint16')/100.0 ;                         % 4#�����׸������(��λ��m)
    
    BTE1(ind)=fread(fid,1,'uint16');                                    % 1#�����׸��ٻز�ǿ��
    BTE2(ind)=fread(fid,1,'uint16');                                    % 2#�����׸��ٻز�ǿ��
    BTE3(ind)=fread(fid,1,'uint16');                                    % 3#�����׸��ٻز�ǿ��
    BTE4(ind)=fread(fid,1,'uint16');                                    % 4#�����׸��ٻز�ǿ��
    
    BTCor1(ind)=fread(fid,1,'uint8');                                   % 1#�����׸��ٻز����ϵ��
    BTCor2(ind)=fread(fid,1,'uint8');                                   % 2#�����׸��ٻز����ϵ��
    BTCor3(ind)=fread(fid,1,'uint8');                                   % 3#�����׸��ٻز����ϵ��
    BTCor4(ind)=fread(fid,1,'uint8');                                   % 4#�����׸��ٻز����ϵ��
    
    BTP1(ind)=fread(fid,1,'uint8');                                     % 1#�����׸������������
    BTP2(ind)=fread(fid,1,'uint8');                                     % 2#�����׸������������
    BTP3(ind)=fread(fid,1,'uint8');                                     % 3#�����׸������������
    BTP4(ind)=fread(fid,1,'uint8');                                     % 4#�����׸������������
    
    fread(fid,14,'uchar');                                              % ����14���ֽ�
    
    %% ***************************���������ݣ������̡��¶ȡ�ѹ����GPS���ݣ�****************** %%
    Sensor_ID{ind}=dec2hex(fread(fid,2,'uchar')) ;                            % ����ͷID: 0x00,0x06
    Pressure(ind)=fread(fid,1,'uint16')/10.0;                            % ѹ������������(��λ:m)
    Temp(ind)=fread(fid,1,'int16')/100.0;                                % �¶ȴ���������(��λ:��)
    Roll(ind)=fread(fid,1,'int16')/10.0;                                 % ��ҡ����(��λ:��)
    Pitch(ind)=fread(fid,1,'int16')/10.0;                                % ��ҡ����(��λ:��)
    Heading(ind)=fread(fid,1,'int16')/100.0;                             % ���������(��λ:��)
    WOE(ind)=fread(fid,1,'uchar');                                       % ���ȱ�־λ
    Long_deg=fread(fid,1,'int16') ;
    Long_sec=fread(fid,2,'int16') ;
    NOS(ind)=fread(fid,1,'uchar');                                       % γ�ȱ�־λ
    Lat_deg=fread(fid,1,'int16') ;
    Lat_sec=fread(fid,2,'int16') ;
    fread(fid,6,'uchar');                                                % ����6���ֽ�
    
    %% *******************************����β��У�������****************************** %%
    Check_ID{ind}=dec2hex(fread(fid,2,'uchar')) ;                              % ����ͷID: 0x00,0x07
    Voltage(ind)=fread(fid,1,'uint16');                                    % ��ѹ���
    fread(fid,10,'uchar');                                                % ����6���ֽ�
    Check_Sum(ind)=fread(fid,1,'uint16');                                 % �����ֽں�
    
    % ind
end
fclose(fid);

vxb=(V1-V2)/2/sind(24.6);
vyb=(V3-V4)/2/sind(24.6);
vx=0.707*(vxb-vyb);
vy=0.707*(vxb+vyb);
Vall=sqrt(vx.^2+vy.^2);

%{
src_data = Vall';
[src_height, src_width] = size(src_data);

des_height = src_height * 10;
des_width = src_width * 10;

des_width = round(src_width /10);
des_data = zeros(des_height, des_width);

for des_row=1:des_height
    src_row=(des_row-1)/(des_height-1)*(src_height-1)+1;
    src_row = round(src_row);
    for des_col=1:des_width
        src_col=(des_col-1)/(des_width-1)*(src_width-1)+1;
        src_col = round(src_col);
        des_data(des_row, des_col) = src_data(src_row, src_col);
    end
end


in = des_data;


min_percent = 2;
max_percent = 100 - min_percent;
tmp = in;
tmp = tmp(:);
min_data = prctile(tmp, min_percent);
max_data = prctile(tmp, max_percent);
data_color = jet(64);
nan_color = [0, 0, 0];
rgb = data2rgb(in, min_data, max_data, data_color, nan_color);


imshow(rgb);
imwrite(rgb,'rgb.png');




 histogram(out(:));


in = Vall';
min_percent = 2;
max_percent = 100 - min_percent;
tmp = in;
tmp = tmp(:);
min_data = prctile(tmp, min_percent);
max_data = prctile(tmp, max_percent);

in(in<min_data) = min_data;
in(in>max_data) = max_data;

 histogram(in(:));



miu = mean(des_data(:));
sigma = std(des_data(:));

min_data = miu - sigma * 3;
max_data = miu + sigma * 3;


color = jet(64);
nan_color = [0,0,0];

rgb = data2rgb_linear(des_data, color, nan_color);

rgb = data2rgb_linear2(des_data, color, nan_color);

imshow(rgb);

imwrite(rgb, 'rgb.png');







histogram(des_data(:))

%}

out_height = 12*11;
out_width = 600;
data = resizeData(E1', out_height, out_width);

rgb = data2rgb(data);
imshow(rgb);




