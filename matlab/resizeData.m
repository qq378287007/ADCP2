function out_data = resizeData(in_data, out_height, out_width)
%{
[in_height, in_width] = size(in_data);
[Y, X] = meshgrid(1:in_width, 1:in_height);

out_height = ((1:out_height)-1) / (out_height-1) * (in_height-1) + 1;
out_width = ((1:out_width)-1) / (out_width-1) * (in_width-1) + 1;

[Yq, Xq] = meshgrid(out_width, out_height);

out_data = interp2(X, Y, in_data, Xq, Yq);
%}

[in_row, in_col] = size(in_data);

out_row = ((1:out_height)-1) / (out_height-1) * (in_row-1) + 1;
out_col = ((1:out_width)-1) / (out_width-1) * (in_col-1) + 1;

out_row = round(out_row);
out_col = round(out_col);
out_data = in_data(out_row, out_col);

