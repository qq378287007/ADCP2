function rgb = data2rgb(data)

data_min = min(data(:));
data_max = max(data(:));

data(data<data_min)=data_min;
data(data>data_max)=data_max;
data=(data-data_min)/(data_max-data_min);

r=data;
g=data;
b=data;

color_number = 64;
color_bar = jet(color_number);

ind = data(:)*(color_number-1)+1;
ind = round(ind);

r(:) = color_bar(ind, 1);
g(:) = color_bar(ind, 2);
b(:) = color_bar(ind, 3);

rgb=cat(3, r, g, b);