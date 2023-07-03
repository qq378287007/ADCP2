clear all;
close all;
clc;
orgdata = load('SLS150_ADCP_2_20211211113000.mat');

vxb=(orgdata.V2-orgdata.V4)/2/sind(24.6);
vyb=(orgdata.V1-orgdata.V3)/2/sind(24.6);
vxorg=0.707*(vxb-vyb);
vyorg=0.707*(vxb+vyb);

ind=1;
for i =1:10:length(orgdata.RTC_year)
    Pres(ind) = mean(orgdata.Pressure(i+1:i+9));
    Temp(ind) = mean(orgdata.Temp(i+1:i+9));
    Roll(ind) = mean(orgdata.Roll(i+1:i+9));
    Pitch(ind) = mean(orgdata.Pitch(i+1:i+9));
    Heading(ind) = mean(orgdata.Heading(i+1:i+9));
    
    RTC_year(ind) = orgdata.RTC_year(i);
    RTC_month(ind) = orgdata.RTC_month(i);
    RTC_day(ind) = orgdata.RTC_day(i);
    RTC_hour(ind) = orgdata.RTC_hour(i);
    RTC_min(ind) = orgdata.RTC_min(i);
    RTC_sec(ind) = orgdata.RTC_sec(i);
    
    
    for j = 1:orgdata.WN
        vx(ind,j) = mean(vxorg(i+1:i+9,j));
        vy(ind,j) = mean(vyorg(i+1:i+9,j));
        E1(ind,j) = mean(orgdata.E1(i+1:i+9,j));
        E2(ind,j) = mean(orgdata.E2(i+1:i+9,j));
        E3(ind,j) = mean(orgdata.E3(i+1:i+9,j));
        E4(ind,j) = mean(orgdata.E4(i+1:i+9,j));
        Cor1(ind,j) = mean(orgdata.Cor1(i+1:i+9,j));
        Cor2(ind,j) = mean(orgdata.Cor2(i+1:i+9,j));
        Cor3(ind,j) = mean(orgdata.Cor3(i+1:i+9,j));
        Cor4(ind,j) = mean(orgdata.Cor4(i+1:i+9,j));
    end
    
    ind =ind+1;
end

ind=1;
ctddata = load('ctd.mat');
for i =1:10:length(ctddata.Date)-10
    ctdDate(ind) = ctddata.Date(i+1);
    ctdTime(ind) = ctddata.Time(i+1);
    ctdPres(ind) = mean(ctddata.Pres(i+1:i+9));
    ctdTemp(ind) = mean(ctddata.Temp(i+1:i+9));
    ctdCond(ind) = mean(ctddata.Cond(i+1:i+9));
    ctdSal(ind) = mean(ctddata.Sal(i+1:i+9));
    ctdSoundV(ind) = mean(ctddata.SoundV(i+1:i+9));
    
    ind =ind+1;
end
ind =  find(ctdPres > 10000);
ctdPres(ind) = ctdPres(ind-1);
ctdTemp(ind) = ctdTemp(ind-1);
ctdCond(ind) = ctdCond(ind-1);
ctdSal(ind) = ctdSal(ind-1);
ctdSoundV(ind) = ctdSoundV(ind-1);

ctdPresPlot = ctdPres(startn:endn);
ctdTempPlot = ctdTemp(startn:endn);
ctdCondPlot = ctdCond(startn:endn);
ctdSalPlot = ctdSal(startn:endn);
ctdSoundVPlot = ctdSoundV(startn:endn);


ctdPresPlot = sigma3smooth(ctdPresPlot,20);
ctdTempPlot = sigma3smooth(ctdTempPlot,20);
ctdCondPlot = sigma3smooth(ctdCondPlot,20);
ctdSalPlot = sigma3smooth(ctdSalPlot,20);

ctdPresPlot = sigma3smooth(ctdPresPlot,20);
ctdTempPlot = sigma3smooth(ctdTempPlot,20);
ctdCondPlot = sigma3smooth(ctdCondPlot,20);
ctdSalPlot = sigma3smooth(ctdSalPlot,20);




kFactor=1;
alpha=0;
for j= 1:orgdata.WN
    Ve(:,j)=kFactor*(vy(:,j).*sind(Heading-alpha)'+vx(:,j).*cosd(Heading-alpha)') ;
    Vn(:,j)=kFactor*(vy(:,j).*cosd(alpha-Heading)'+vx(:,j).*sind(alpha-Heading)') ;
end

v=sqrt(Ve.*Ve+Vn.*Vn);
angle = atan2(Ve,Vn)*180/3.1415926;
angle(angle<0) = angle(angle<0)+360;

timeVector = [RTC_year+2000;RTC_month;RTC_day;RTC_hour;RTC_min;RTC_sec]';
timeStr =datestr(timeVector,'yyyy-mm-dd HH:MM:SS');

datenum(timeVector);


save('ADCP_1_2.mat','Heading','Pitch','Roll','Pres','Temp','Ve','Vn','timeVector');


startn = find(Pres > 5,1,'first')+5;
endn = find(Pres > 5,1,'last')-5;

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
plottimePlot =plottime(startn:endn);
for i=1:length(plottimePlot)
    plottimePlottemp(i) = plottimePlot(1)+(plottimePlot(2)-plottimePlot(1))*(i-1)*1.08;
end
plottimePlot = plottimePlottemp;
plottimePlot(end)-plottimePlot(1)


figure(1)
plot(plottimePlot,TempPlot+mean(ctdTempPlot)-mean(TempPlot),'r','LineWidth',1.5)
hold on
plot(plottimePlot,ctdTempPlot,'b','LineWidth',1.5)
h = legend('ADCP测量温度','CTD测量温度');
xlabel('时间','FontSize',15)
ylabel('温度（℃）','FontSize',15)
set(gca,'FontSize',15)
set(h,'FontSize',15)
dateaxis('x',17)
grid on;
title('ADCP与CTD测量温度比对图','FontSize',18)
saveas(gcf,'ADCP与CTD测量温度比对图.jpg')




figure(2)
plot(plottimePlot,PresPlot+mean(ctdPresPlot)-mean(PresPlot),'r','LineWidth',1.5)
hold on
plot(plottimePlot,ctdPresPlot,'b','LineWidth',1.5)
h = legend('ADCP测量深度','CTD测量深度');
xlabel('时间','FontSize',15)
ylabel('深度（m）','FontSize',15)
set(gca,'FontSize',15)
set(h,'FontSize',15)
dateaxis('x',17)
grid on;
title('ADCP与CTD测量深度比对图','FontSize',18)
saveas(gcf,'ADCP与CTD测量深度比对图.jpg')


figure(3)
plot(plottimePlot,RollPlot,'b','LineWidth',1.5)
xlabel('时间','FontSize',15)
ylabel('横摇（°）','FontSize',15)
set(gca,'FontSize',15)
dateaxis('x',17)
grid on;
title('ADCP测量横摇数据图','FontSize',18)
saveas(gcf,'ADCP测量横摇数据图.jpg')

figure(4)
plot(plottimePlot,PitchPlot,'b','LineWidth',1.5)
xlabel('时间','FontSize',15)
ylabel('纵倾（°）','FontSize',15)
set(gca,'FontSize',15)
dateaxis('x',17)
grid on;
title('ADCP测量纵倾数据图','FontSize',18)
saveas(gcf,'ADCP测量纵倾数据图.jpg')

figure(5)
plot(plottimePlot,HeadingPlot,'b','LineWidth',1.5)
xlabel('时间','FontSize',15)
ylabel('航向（°）','FontSize',15)
set(gca,'FontSize',15)
dateaxis('x',17)
grid on;
title('ADCP测量航向数据图','FontSize',18)
saveas(gcf,'ADCP测量航向数据图.jpg')


for i=1:length(ctdCondPlot)
    if(ctdCondPlot(i)<-100 )
        ctdCondPlot(i)= ctdCondPlot(i-1);
    end
end
figure(6)
plot(plottimePlot,ctdCondPlot,'b','LineWidth',1.5)
xlabel('时间','FontSize',15)
ylabel('电导率（mS/cm）','FontSize',15)
set(gca,'FontSize',15)
dateaxis('x',17)
grid on;
title('CTD测量电导率数据图','FontSize',18)
saveas(gcf,'CTD测量电导率数据图.jpg')

figure(7)
plot(plottimePlot,ctdSalPlot,'b','LineWidth',1.5)
xlabel('时间','FontSize',15)
ylabel('盐度（‰）','FontSize',15)
set(gca,'FontSize',15)
dateaxis('x',17)
grid on;
title('CTD测量盐度数据图','FontSize',18)
saveas(gcf,'CTD测量盐度数据图.jpg')


for cell = [2,5,7]
    figure(4*cell+6)
    plot(plottimePlot,vPlot(:,cell),'b','LineWidth',1)
    xlabel('时间','FontSize',15)
    ylabel('流速（m/s）','FontSize',15)
    set(gca,'FontSize',15)
    dateaxis('x',17)
    grid on;
    title(['ADCP测量第',num2str(cell),'层流速数据图'],'FontSize',18)
    saveas(gcf,['ADCP测量第',num2str(cell),'层流速数据图.jpg'])
    
    figure(4*cell+7)
    plot(plottimePlot,anglePlot(:,cell),'b','LineWidth',1)
    xlabel('时间','FontSize',15)
    ylabel('流向（°）','FontSize',15)
    set(gca,'FontSize',15)
    dateaxis('x',17)
    grid on;
    title(['ADCP测量第',num2str(cell),'层流向数据图'],'FontSize',18)
    saveas(gcf,['ADCP测量第',num2str(cell),'层流向数据图.jpg'])
    
    
    figure(4*cell+8)
    plot(plottimePlot(15600:16200),vPlot(15600:16200,cell),'b','LineWidth',1)
    xlabel('时间','FontSize',15)
    ylabel('流速（m/s）','FontSize',15)
    set(gca,'FontSize',15)
    dateaxis('x',17)
    grid on;
    title(['ADCP测量第',num2str(cell),'层流速数据图(局部)'],'FontSize',18)
    saveas(gcf,['ADCP测量第',num2str(cell),'层流速数据图(局部).jpg'])
    
    figure(4*cell+9)
    plot(plottimePlot(15600:16200),anglePlot(15600:16200,cell),'b','LineWidth',1)
    xlabel('时间','FontSize',15)
    ylabel('流向（°）','FontSize',15)
    set(gca,'FontSize',15)
    dateaxis('x',17)
    grid on;
    title(['ADCP测量第',num2str(cell),'层流向数据图(局部)'],'FontSize',18)
    saveas(gcf,['ADCP测量第',num2str(cell),'层流向数据图(局部).jpg'])
    
end
% close all
