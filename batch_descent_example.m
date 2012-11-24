function [b,errorIteration] = batch_descent_example(descent_X, descent_y, iterations)
lambda = 1;
episilon = 1e-4;

numFeatures = size(descent_X,2);
numExamples = size(descent_X,1);
b = 0.01*rand(numFeatures,1);

yhat = descent_X*b;
totalErr = sum((descent_y - yhat).^2);
fprintf(1,'Initial error: %f\n', totalErr);

for t = 1:iterations
  fprintf('***Iteration: %d***\n', t);
  
  gradsum = 0;
  for i = 1:numExamples
    x = descent_X(i,:);
    y = descent_y(i,1);
    % Compute first part of update here
    %gradsum = gradsum + ________
    %
  end
  % Compute remaing part of update here
  % gradsum = gradsum + _______;
  
    b = b - episilon*gradsum;
  
  % Now we'll use our data to fit the data
  yhat = descent_X*b;
  totalErr = sum((descent_y - yhat).^2);
  errorIteration(t) = totalErr;
  fprintf(1,'Error: %f\n', totalErr);
end
