load descent_regression.mat

[b, errorIterationStochastic] = stochastic_descent_example(descent_X, descent_y, 50);

[b, errorIterationBatch] = batch_descent_example(descent_X, descent_y, 50);

% plot as iterations
plot(1:50, errorIterationStochastic);
hold on
plot(1:50, errorIterationBatch, 'r');
legend('Stochastic', 'Batch');

