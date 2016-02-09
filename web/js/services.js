// ENVIRONMENT

app.factory('Environment', ['$http', function($http)
{
    return {
        'get': function()
        {
            return $http({
                method: 'GET',
                url: 'environment'

            });
        },
        'update': function(data){
            return $http({
                method: 'PUT',
                url: 'environment',
                data: data
            });
        }
    }
}]);

// AGENTS

app.factory('Agent', ['$http', function($http)
{
    return {
        'create': function(class_name, data)
        {
            return $http({
                method: 'POST',
                url: 'agents/' + class_name,
                data: data
            });
        },
        'update': function(class_name, id, data)
        {
            return $http({
                method: 'PUT',
                url: 'agents/' + class_name + '/' + id,
                data : data
            });
        },
        'deleteAll': function(class_name, id)
        {
            return $http({
                method: 'DELETE',
                url: 'agents/' + class_name
            });
        },
        'deleteOne': function(class_name, id)
        {
            return $http({
                method: 'DELETE',
                url: 'agents/' + class_name + '/' + id
            });
        },
        'getAll': function(class_name , from , to)
        {
            return $http({
                method: 'GET',
                url: 'agents/' + class_name + '/from/' + from + '/to/' + to
            });
        },
        'getOne': function(class_name, id)
        {
            return $http({
                method: 'GET',
                url: 'agents/' + class_name + '/' + id
            });
        },
        'runAll': function(class_name)
        {
            return $http({
                method: 'POST',
                url: 'agents/' + class_name + '/run'
            });
        },
        'runOne': function(class_name, id)
        {
            return $http({
                method: 'POST',
                url: 'agents/' + class_name + '/' + id + '/run'
            });
        },
        'stopAll': function(class_name)
        {
            return $http({
                method: 'POST',
                url: 'agents/' + class_name + '/stop'
            });
        },
        'stopOne': function(class_name, id)
        {
            return $http({
                method: 'POST',
                url: 'agents/' + class_name + '/' + id + '/stop'
            });
        }
    }
}]);