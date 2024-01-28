import PropTypes from 'prop-types';
import fan from '../assets/fan.svg';

const Fan = ({ ventiladorEncendido }) => {
	return (
		<div className='h-52 w-52 shadow-xl rounded-2xl flex flex-col gap-5 justify-center items-center col-start-2 row-start-2'>
			<img
				src={fan}
				alt=''
				className={`h-20 w-20 ${
					ventiladorEncendido ? 'animate-spin' : ''
				}`}
			/>
			<p className='text-4xl'>
				{ventiladorEncendido ? 'Encendido' : 'Apagado'}
			</p>
		</div>
	);
};

Fan.propTypes = {
	ventiladorEncendido: PropTypes.bool.isRequired,
};

export default Fan;
