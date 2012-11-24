function [b, errorIteration] = stochastic_descent_example(descent_X, descent_y, iterations)
lambda = 1;
episilon = 1e-4;

numFeatures = size(descent_X,2);
numExamples = size(descent_X,1);
b = 0.01*rand(numFeatures,1);

yhat = descent_X*b;
totalErr = sum((descent_y - yhat).^2);
fprintf(1,'Initial error: %f\n', totalErr);

for t = 1:iterations
  fprintf('***Iteration: %d\n***', t);
  
  for i = 1:numExamples
    x = descent_X(i,:);
    y = descent_y(i,1);
    % Compute update rule here
    %b = b - epsilon*_____________
    
    yhat = descent_X*b;
    totalErr = sum((descent_y - yhat).^2);
    fprintf(1,'Error: %f\n', totalErr);
  end
  errorIteration(t) = totalErr;
end
