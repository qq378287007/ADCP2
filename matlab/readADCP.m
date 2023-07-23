close all;clear all;clc;

Name = 'SLS150_ADCP_2_20211211113000.dat';
fid = fopen(Name);

fseek(fid,38,'bof');
cellNum = fread(fid, 1, 'uint8');%����
pingLength = 32 + 32 + (2+cellNum*8) * 2 + (2 + cellNum * 4) + 48 + 32 + 16;%֡��

fseek(fid,0,'eof');
count = ftell(fid);%�ļ��ֽ���
Ping_Num =count / (32 + 32 + 2 * (cellNum * 8 + 2) + (cellNum * 4 + 2) + 96);%֡��

%����ͷ�ļ�
Data_ID = cell(Ping_Num, 1);
Ping_byte = zeros(Ping_Num, 1);

%������������
Para_ID = cell(Ping_Num, 1);
RTC_year = zeros(Ping_Num, 1);
RTC_month = zeros(Ping_Num, 1);
RTC_day = zeros(Ping_Num, 1);
RTC_hour = zeros(Ping_Num, 1);
RTC_min = zeros(Ping_Num, 1);
RTC_sec = zeros(Ping_Num, 1);

%������������
Current_ID = cell(Ping_Num, 1);
V1 = zeros(Ping_Num, cellNum);
V2 = zeros(Ping_Num, cellNum);
V3 = zeros(Ping_Num, cellNum);
V4 = zeros(Ping_Num, cellNum);

%�ز�ǿ����������
Intensity_ID = cell(Ping_Num, 1);
E1 = zeros(Ping_Num, cellNum);
E2 = zeros(Ping_Num, cellNum);
E3 = zeros(Ping_Num, cellNum);
E4 = zeros(Ping_Num, cellNum);

%���ϵ����������
Correlation_ID = cell(Ping_Num, 1);
Cor1 = zeros(Ping_Num, cellNum);
Cor2 = zeros(Ping_Num, cellNum);
Cor3 = zeros(Ping_Num, cellNum);
Cor4 = zeros(Ping_Num, cellNum);

%�׸�������
BottomTrack_ID = cell(Ping_Num, 1);
BTV1 = zeros(Ping_Num, 1);
BTV2 = zeros(Ping_Num, 1);
BTV3 = zeros(Ping_Num, 1);
BTV4 = zeros(Ping_Num, 1);

BTDepth1 = zeros(Ping_Num, 1);
BTDepth2 = zeros(Ping_Num, 1);
BTDepth3 = zeros(Ping_Num, 1);
BTDepth4 = zeros(Ping_Num, 1);

BTE1 = zeros(Ping_Num, 1);
BTE2 = zeros(Ping_Num, 1);
BTE3 = zeros(Ping_Num, 1);
BTE4 = zeros(Ping_Num, 1);

BTCor1 = zeros(Ping_Num, 1);
BTCor2 = zeros(Ping_Num, 1);
BTCor3 = zeros(Ping_Num, 1);
BTCor4 = zeros(Ping_Num, 1);

BTP1 = zeros(Ping_Num, 1);
BTP2 = zeros(Ping_Num, 1);
BTP3 = zeros(Ping_Num, 1);
BTP4 = zeros(Ping_Num, 1);

%���������ݣ������̡��¶ȡ�ѹ����GPS���ݣ�
Sensor_ID = cell(Ping_Num, 1);
Pressure = zeros(Ping_Num, 1);
Temp     = zeros(Ping_Num, 1);
Roll     = zeros(Ping_Num, 1);
Pitch    = zeros(Ping_Num, 1);
Heading  = zeros(Ping_Num, 1);
WOE  = zeros(Ping_Num, 1);
%Long_deg  = zeros(Ping_Num, 1);
%Long_sec  = zeros(Ping_Num, 1);
NOS  = zeros(Ping_Num, 1);
%Lat_deg  = zeros(Ping_Num, 1);
%Lat_sec  = zeros(Ping_Num, 1);

%����β��У�������
Check_ID = cell(Ping_Num, 1);
Voltage  = zeros(Ping_Num, 1);
Check_Sum  = zeros(Ping_Num, 1);

fseek(fid,0,'bof');
%Ping_Num = 500;
for ind = 1 : Ping_Num
    %% *******************************����ͷ�ļ�************************************** %%
    Data_ID{ind} = dec2hex(fread(fid, 2, 'uchar')) ;                   % ����ͷID: 0x7F,0x7F
    Ping_byte(ind) = fread(fid, 1, 'uint16') ;                         % ֡��: 32+32+(2+cellNum*8)*2+(2+cellNum*4)+48+32+16
    Offset1 = fread(fid, 1, 'uint16') ;                                % Offset1
    Offset2 = fread(fid, 1, 'uint16') ;                                % Offset2
    Offset3 = fread(fid, 1, 'uint16') ;                                % Offset3
    Offset4 = fread(fid, 1, 'uint16') ;                                % Offset4
    Offset5 = fread(fid, 1, 'uint16') ;                                % Offset5
    Offset6 = fread(fid, 1, 'uint16') ;                                % Offset6
    Offset7 = fread(fid, 1, 'uint16') ;                                % Offset7
    fread(fid, 14, 'uchar');                                         % 14�������ֽ�

    %% ******************************������������*************************************** %%
    Para_ID{ind} = dec2hex(fread(fid, 2, 'uchar')) ;                        % ����ͷID: 0x00,0x01
    WF = fread(fid, 1, 'uint16')/100 ;                                      % ä��(m)
    WS = fread(fid, 1, 'uint16')/100 ;                                      % ���(m)
    WN = fread(fid, 1, 'uint8') ;                                           % ����
    WP = fread(fid, 1, 'uint16') ;                                          % ����
    TP_min = fread(fid, 1, 'uint8') ;                                       % �����ڣ�min��
    TP_sec = fread(fid, 1, 'uint8') ;                                       % �����ڣ�Sec��

    TF_year = fread(fid, 1, 'uint8') ;                                      % TFʱ�䣨�꣩
    TF_month = fread(fid, 1, 'uint8') ;                                     % TFʱ�䣨�£�
    TF_day = fread(fid, 1, 'uint8') ;                                       % TFʱ�䣨�գ�
    TF_hour = fread(fid, 1, 'uint8') ;                                      % TFʱ�䣨ʱ��
    TF_min = fread(fid, 1, 'uint8') ;                                       % TFʱ�䣨�֣�
    TF_sec = fread(fid, 1, 'uint8') ;                                       % TFʱ�䣨�룩

    TE_hour = fread(fid, 1, 'uint8') ;                                      % TEʱ�䣨ʱ��
    TE_min = fread(fid, 1, 'uint8') ;                                       % TEʱ�䣨�֣�
    TE_sec = fread(fid, 1, 'uint8') ;                                       % TEʱ�䣨�룩

    TB_hour = fread(fid, 1, 'uint8') ;                                      % TBʱ�䣨ʱ��
    TB_min = fread(fid, 1, 'uint8') ;                                       % TBʱ�䣨�֣�
    TB_sec = fread(fid, 1, 'uint8') ;                                       % TBʱ�䣨�룩

    TC = fread(fid, 1, 'uint16') ;                                          % TC��Ŀ

    RTC_year(ind) = fread(fid, 1, 'uint8') ;                                % ��¼ʱ�䣨�꣩
    RTC_month(ind) = fread(fid, 1, 'uint8') ;                               % ��¼ʱ�䣨�£�
    RTC_day(ind) = fread(fid, 1, 'uint8') ;                                 % ��¼ʱ�䣨�գ�
    RTC_hour(ind) = fread(fid, 1, 'uint8') ;                                % ��¼ʱ�䣨ʱ��
    RTC_min(ind) = fread(fid, 1, 'uint8') ;                                 % ��¼ʱ�䣨�֣�
    RTC_sec(ind) = fread(fid, 1, 'uint8') ;                                 % ��¼ʱ�䣨�룩
    fread(fid, 1, 'uchar');                                               % 1�������ֽ�

    %% *******************************������������************************************ %%
    Current_ID{ind} = dec2hex(fread(fid, 2, 'uchar')) ;                               % ����ͷID: 0x00,0x02
    for index = 1 : cellNum
        V1(ind, index)= fread(fid, 1, 'int16=>double')/1000;                    % ��index��1#�����ٶ�(��λ:mm/s)
        V2(ind, index)= fread(fid, 1, 'int16=>double')/1000;                    % ��index��2#�����ٶ�(��λ:mm/s)
        V3(ind, index)= fread(fid, 1, 'int16=>double')/1000;                    % ��index��3#�����ٶ�(��λ:mm/s)
        V4(ind, index)= fread(fid, 1, 'int16=>double')/1000;                    % ��index��4#�����ٶ�(��λ:mm/s)
    end

    %% *****************************�ز�ǿ����������*********************************** %%
    Intensity_ID{ind} = dec2hex(fread(fid, 2, 'uchar')) ;                        % ����ͷID: 0x00,0x03
    for index = 1 : cellNum
        E1(ind, index)= fread(fid, 1, 'uint16');                           % ��index��1#�����ز�ǿ��
        E2(ind, index)= fread(fid, 1, 'uint16');                           % ��index��2#�����ز�ǿ��
        E3(ind, index)= fread(fid, 1, 'uint16');                           % ��index��3#�����ز�ǿ��
        E4(ind, index)= fread(fid, 1, 'uint16');                           % ��index��4#�����ز�ǿ��
    end

    %% *****************************���ϵ����������*********************************** %%
    Correlation_ID{ind} = dec2hex(fread(fid, 2, 'uchar')) ;                      % ����ͷID: 0x00,0x04
    for index = 1 : cellNum
        Cor1(ind, index) = fread(fid, 1, 'uint8') ;                          % 1#�������ϵ��
        Cor2(ind, index) = fread(fid, 1, 'uint8') ;                          % 2#�������ϵ��
        Cor3(ind, index) = fread(fid, 1, 'uint8') ;                          % 3#�������ϵ��
        Cor4(ind, index) = fread(fid, 1, 'uint8') ;                          % 4#�������ϵ��
    end

    %% *******************************�׸�������************************************** %%
    BottomTrack_ID{ind}=  dec2hex(fread(fid, 2, 'uchar')) ;                      % ����ͷID: 0x00,0x05

    BTV1(ind) = fread(fid, 1, 'int16=>double') ;                            % 1#�����׸����ٶ�(��λ:mm/s)
    BTV2(ind) = fread(fid, 1, 'int16=>double') ;                            % 2#�����׸����ٶ�(��λ:mm/s)
    BTV3(ind) = fread(fid, 1, 'int16=>double') ;                            % 3#�����׸����ٶ�(��λ:mm/s)
    BTV4(ind) = fread(fid, 1, 'int16=>double') ;                            % 4#�����׸����ٶ�(��λ:mm/s)

    BTDepth1(ind) = fread(fid, 1, 'uint16')/100.0 ;                         % 1#�����׸������(��λ��m)
    BTDepth2(ind) = fread(fid, 1, 'uint16')/100.0 ;                         % 2#�����׸������(��λ��m)
    BTDepth3(ind) = fread(fid, 1, 'uint16')/100.0 ;                         % 3#�����׸������(��λ��m)
    BTDepth4(ind) = fread(fid, 1, 'uint16')/100.0 ;                         % 4#�����׸������(��λ��m)

    BTE1(ind) = fread(fid, 1, 'uint16');                                    % 1#�����׸��ٻز�ǿ��
    BTE2(ind) = fread(fid, 1, 'uint16');                                    % 2#�����׸��ٻز�ǿ��
    BTE3(ind) = fread(fid, 1, 'uint16');                                    % 3#�����׸��ٻز�ǿ��
    BTE4(ind) = fread(fid, 1, 'uint16');                                    % 4#�����׸��ٻز�ǿ��

    BTCor1(ind) = fread(fid, 1, 'uint8');                                   % 1#�����׸��ٻز����ϵ��
    BTCor2(ind) = fread(fid, 1, 'uint8');                                   % 2#�����׸��ٻز����ϵ��
    BTCor3(ind) = fread(fid, 1, 'uint8');                                   % 3#�����׸��ٻز����ϵ��
    BTCor4(ind) = fread(fid, 1, 'uint8');                                   % 4#�����׸��ٻز����ϵ��

    BTP1(ind) = fread(fid, 1, 'uint8');                                     % 1#�����׸������������
    BTP2(ind) = fread(fid, 1, 'uint8');                                     % 2#�����׸������������
    BTP3(ind) = fread(fid, 1, 'uint8');                                     % 3#�����׸������������
    BTP4(ind) = fread(fid, 1, 'uint8');                                     % 4#�����׸������������

    fread(fid, 14, 'uchar');                                              % ����14���ֽ�

    %% ***************************���������ݣ������̡��¶ȡ�ѹ����GPS���ݣ�****************** %%
    Sensor_ID{ind} = dec2hex(fread(fid, 2, 'uchar')) ;                            % ����ͷID: 0x00,0x06
    Pressure(ind) = fread(fid, 1, 'uint16')/10.0;                            % ѹ������������(��λ:m)
    Temp(ind) = fread(fid, 1, 'int16')/100.0;                                % �¶ȴ���������(��λ:��)
    Roll(ind) = fread(fid, 1, 'int16')/10.0;                                 % ��ҡ����(��λ:��)
    Pitch(ind) = fread(fid, 1, 'int16')/10.0;                                % ��ҡ����(��λ:��)
    Heading(ind) = fread(fid, 1, 'int16')/100.0;                             % ���������(��λ:��)
    WOE(ind) = fread(fid, 1, 'uchar');                                       % ���ȱ�־λ
    Long_deg= fread(fid, 1, 'int16') ;
    Long_sec= fread(fid, 2, 'int16') ;
    NOS(ind) = fread(fid, 1, 'uchar');                                       % γ�ȱ�־λ
    Lat_deg= fread(fid, 1, 'int16') ;
    Lat_sec= fread(fid, 2, 'int16') ;
    fread(fid, 6, 'uchar');                                                % ����6���ֽ�

    %% *******************************����β��У�������****************************** %%
    Check_ID{ind} = dec2hex(fread(fid, 2, 'uchar')) ;                              % ����ͷID: 0x00,0x07
    Voltage(ind) = fread(fid, 1, 'uint16');                                    % ��ѹ���
    fread(fid, 10, 'uchar');                                                % ����6���ֽ�
    Check_Sum(ind) = fread(fid, 1, 'uint16');                                 % �����ֽں�

    % ind
end
fclose(fid);

vxb = (V2 - V4) / 2 / sind(24.6);
vyb = (V1 - V3) / 2 / sind(24.6);
vxorg = 0.707 * (vxb - vyb);
vyorg = 0.707 * (vxb + vyb);

%1��10������
%{
ind=1;
for i =1:10:length(RTC_year)
    Pres(ind) = mean(Pressure(i+1:i+9));
    Temp(ind) = mean(Temp(i+1:i+9));
    Roll(ind) = mean(Roll(i+1:i+9));
    Pitch(ind) = mean(Pitch(i+1:i+9));
    Heading(ind) = mean(Heading(i+1:i+9));
    
    RTC_year(ind) = RTC_year(i);
    RTC_month(ind) = RTC_month(i);
    RTC_day(ind) = RTC_day(i);
    RTC_hour(ind) = RTC_hour(i);
    RTC_min(ind) = RTC_min(i);
    RTC_sec(ind) = RTC_sec(i);

    for j = 1:WN
        vx(ind,j) = mean(vxorg(i+1:i+9,j));
        vy(ind,j) = mean(vyorg(i+1:i+9,j));
        E1(ind,j) = mean(E1(i+1:i+9,j));
        E2(ind,j) = mean(E2(i+1:i+9,j));
        E3(ind,j) = mean(E3(i+1:i+9,j));
        E4(ind,j) = mean(E4(i+1:i+9,j));
        Cor1(ind,j) = mean(Cor1(i+1:i+9,j));
        Cor2(ind,j) = mean(Cor2(i+1:i+9,j));
        Cor3(ind,j) = mean(Cor3(i+1:i+9,j));
        Cor4(ind,j) = mean(Cor4(i+1:i+9,j));
    end
    
    ind =ind+1;
end
%}

Pres = Pressure;
vx = vxorg;
vy = vyorg;

HEADING = repmat(Heading, [1 WN]);
kFactor = 1;
alpha = 0;
Ve = kFactor * (vy .* sind(Heading - alpha) + vx .* cosd(Heading - alpha)) ;
Vn = kFactor * (vy .* cosd(alpha - Heading) + vx .* sind(alpha - Heading)) ;

v = sqrt(Ve .* Ve + Vn .* Vn);
angle = atan2(Ve, Vn) * 180 / pi;
angle(angle<0) = angle(angle<0) + 360;

timeVector = [RTC_year + 2000 RTC_month RTC_day RTC_hour RTC_min RTC_sec];
timeStr = datestr(timeVector, 'yyyy-mm-dd HH:MM:SS');

datenum(timeVector);

startn = find(Pres > 5, 1, 'first') + 5;
endn = find(Pres > 5, 1, 'last') - 5;

timeStr(startn,:)
timeStr(endn,:)


vPlot=v(startn:endn,:);
anglePlot=angle(startn:endn,:);
E1Plot=E1(startn:endn,:);
E2Plot=E2(startn:endn,:);
E3Plot=E3(startn:endn,:);
E4Plot=E4(startn:endn,:);
Cor1Plot=Cor1(startn:endn,:);
Cor2Plot=Cor2(startn:endn,:);
Cor3Plot=Cor3(startn:endn,:);
Cor4Plot=Cor4(startn:endn,:);

PresPlot = Pres(startn:endn);
TempPlot = Temp(startn:endn);
RollPlot = Roll(startn:endn);
PitchPlot = Pitch(startn:endn);
HeadingPlot = Heading(startn:endn);





plottime = datenum(RTC_year+2000,RTC_month,RTC_day,RTC_hour,RTC_min,RTC_sec);
plottimePlot = plottime(startn:endn);
%{
plottimePlottemp = plottimePlot;
for i=1:length(plottimePlot)
    plottimePlottemp(i) = plottimePlot(1)+(plottimePlot(2)-plottimePlot(1))*(i-1)*1.08;
end
plottimePlot = plottimePlottemp;
%}
plottimePlot(end)-plottimePlot(1)


figure(3)
plot(plottimePlot, RollPlot,'b','LineWidth',1.5)
xlabel('ʱ��','FontSize',15)
ylabel('��ҡ���㣩','FontSize',15)
set(gca,'FontSize',15)
dateaxis('x',17)
grid on;
title('ADCP������ҡ����ͼ','FontSize',18)


figure(4)
plot(plottimePlot, PitchPlot, 'b', 'LineWidth', 1.5)
xlabel('ʱ��','FontSize',15)
ylabel('���㣨�㣩','FontSize',15)
set(gca,'FontSize',15)
dateaxis('x',17)
grid on;
title('ADCP������������ͼ','FontSize',18)


figure(5)
plot(plottimePlot,HeadingPlot,'b','LineWidth',1.5)
xlabel('ʱ��','FontSize',15)
ylabel('���򣨡㣩','FontSize',15)
set(gca,'FontSize',15)
dateaxis('x',17)
grid on;
title('ADCP������������ͼ','FontSize',18)

for cell = [2,5,7]
    figure(4*cell+6)
    plot(plottimePlot,vPlot(:,cell),'b','LineWidth',1)
    xlabel('ʱ��','FontSize',15)
    ylabel('���٣�m/s��','FontSize',15)
    set(gca,'FontSize',15)
    dateaxis('x',17)
    grid on;
    title(['ADCP������',num2str(cell),'����������ͼ'],'FontSize',18)
    %saveas(gcf,['ADCP������',num2str(cell),'����������ͼ.jpg'])
    
    figure(4*cell+7)
    plot(plottimePlot,anglePlot(:,cell),'b','LineWidth',1)
    xlabel('ʱ��','FontSize',15)
    ylabel('���򣨡㣩','FontSize',15)
    set(gca,'FontSize',15)
    dateaxis('x',17)
    grid on;
    title(['ADCP������',num2str(cell),'����������ͼ'],'FontSize',18)
    %saveas(gcf,['ADCP������',num2str(cell),'����������ͼ.jpg'])
    
    
    figure(4*cell+8)
    plot(plottimePlot(15600:16200),vPlot(15600:16200,cell),'b','LineWidth',1)
    xlabel('ʱ��','FontSize',15)
    ylabel('���٣�m/s��','FontSize',15)
    set(gca,'FontSize',15)
    dateaxis('x',17)
    grid on;
    title(['ADCP������',num2str(cell),'����������ͼ(�ֲ�)'],'FontSize',18)
    %saveas(gcf,['ADCP������',num2str(cell),'����������ͼ(�ֲ�).jpg'])
    
    figure(4*cell+9)
    plot(plottimePlot(15600:16200),anglePlot(15600:16200,cell),'b','LineWidth',1)
    xlabel('ʱ��','FontSize',15)
    ylabel('���򣨡㣩','FontSize',15)
    set(gca,'FontSize',15)
    dateaxis('x',17)
    grid on;
    title(['ADCP������',num2str(cell),'����������ͼ(�ֲ�)'],'FontSize',18)
    %saveas(gcf,['ADCP������',num2str(cell),'����������ͼ(�ֲ�).jpg'])
    
end

%{
vxb = (V1 - V2) / 2 / sind(24.6);
vyb = (V3 - V4) / 2 / sind(24.6);
vx = 0.707 * (vxb - vyb);
vy = 0.707 * (vxb + vyb);
Vall = sqrt(vx .^ 2 + vy .^ 2);
%}

%{
Name(end-2)='m';
Name(end-1)='a';
Name(end)='t';
save(Name,'WF','WS','WN','WP',...
    'TP_min','TP_sec','TF_year','TF_month','TF_day','TF_hour','TF_min','TF_sec','TE_hour','TE_min','TE_min',...
    'TB_hour','TB_min','TB_sec','TC','RTC_year','RTC_month','RTC_day','RTC_hour','RTC_min','RTC_sec',...
    'V1','V2','V3','V4','E1','E2','E3','E4','Cor1','Cor2','Cor3','Cor4','BTV1','BTV2','BTV3','BTV4',...
    'BTDepth1','BTDepth2','BTDepth3','BTDepth4','BTE1','BTE2','BTE3','BTE4',...
    'BTCor1','BTCor2','BTCor3','BTCor4','Pressure','Temp','Roll','Pitch','Heading','Vall');
%}



%{
cell = 1; % �ο����ͼ��ѡֵ��Χ1-cell_Num
figure(1);
grid on;
hold on;
plot(V1(:,cell),'k');
plot(V2(:,cell),'b');
plot(V3(:,cell),'c');
plot(V4(:,cell),'r');
xlabel('������');
ylabel('�����ٶ�(mm/s)');
legend('V1','V2','V3','V4');

index=2;     % index���ϵ��������������ٷֲ�
figure(2);
plot(V1(index,:),'r');
xlabel('���浥Ԫ��');
ylabel('�����ٶ�(mm/s)');


v = V1;
    
AveV=[];
StdV=[];
    
for index=1:cellNum
    CellRef=index;
    AveV=[AveV,mean(v(10:200,CellRef))];
    StdV=[StdV,std(v(10:200,CellRef))];
    E1Ave(index)=mean(E1(10:200,CellRef));
    E2Ave(index)=mean(E2(10:200,CellRef));
    E3Ave(index)=mean(E3(10:200,CellRef));
    E4Ave(index)=mean(E4(10:200,CellRef));
    figure(3)
    plot(v(:,CellRef));

    xlabel('��������')
    ylabel('�ٶ�mm/s')
    title(['��',num2str(CellRef),'��������ٶ�ģֵ'])
    pause;
end
AveV=AveV';
StdV=StdV';

Distance=WF+WS*[1:WN];

figure
grid on;
plot(Distance,AveV)
xlabel('�������(m)');
ylabel('�ٶ�ģֵ(mm/s)');
title('����������ٶȾ�ֵ')

figure
grid on;
plot(Distance,StdV)
xlabel('�������(m)');
ylabel('�ٶȷ���(mm/s)');
title('����������ٶȷ���')

EAve=(E1Ave+E2Ave+E3Ave+E4Ave)/4;
EAve=EAve';
%}

